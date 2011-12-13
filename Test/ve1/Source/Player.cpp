#include "Player.h"
#include <AmjuGL.h>
#include <Teapot.h>
#include "Ve1SceneGraph.h"
#include <iostream>
#include "Ve1Character.h"
#include <File.h>
#include "Useful.h"

namespace Amju
{
class PlayerSceneNode : public Ve1Character
{
public:
  PlayerSceneNode(Player* p) : m_player(p)
  {
  }

  virtual void Draw()
  {
    Ve1Character::Draw();
  }

protected:
  Player* m_player;
};

const char* Player::TYPENAME = "player";

Player::Player() : m_posHasBeenSet(false), m_sceneNode(0)
{
}

bool Player::Load(File* f)
{
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);

  PlayerSceneNode* psn = new PlayerSceneNode(this);
  m_sceneNode = psn;
  
  std::string meshName;
  if (!f->GetDataLine(&meshName))
  {
    f->ReportError("No mesh name for player");
    return false;
  }

  // Load mesh and textures from file
  if (!psn->LoadMd2(meshName))
  {
    return false;
  }

  std::string tex1Name, tex2Name;
  if (!f->GetDataLine(&tex1Name) || !f->GetDataLine(&tex2Name))
  {
    f->ReportError("Failed to get 2 textures for player");
    return false;
  }

  if (!psn->LoadTextures(tex1Name, tex2Name))
  {
    return false;
  }

  root->AddChild(psn);

#ifdef PLAYER_LOAD_DEBUG
std::cout << "Loaded player ok!\n";
#endif

  return true;
}

void Player::Draw()
{
}

void Player::MoveTo(const Vec3f& newpos)
{
  m_newPos = newpos;
  if (m_posHasBeenSet)
  {
std::cout << "Player: got new pos to move to: " << newpos << ", current pos is " << GetPos() << "\n";

    Vec3f dir = GetPos() - newpos;
    if (dir.SqLen() < 1.0f) // TODO CONFIG
    {
      SetVel(Vec3f(0, 0, 0));
    }
    else
    {
      dir.Normalise();
      static const float SPEED = 50.0f; // TODO CONFIG
      SetVel(-dir * SPEED);
    }
  }
  else  
  {
    // First pos update, so immediately set pos
    m_posHasBeenSet = true;
    SetPos(newpos); 
  }
}

void Player::Update()
{
  if (m_sceneNode)
  {
    Matrix m;
    m.Translate(m_pos);
    m_sceneNode->SetLocalTransform(m);
    m_sceneNode->Update();
  }

  // Move to new pos ?
//  float sqd = (GetPos() - m_newPos).SqLen();
//  Vec3f vel = m_newPos - GetPos();
//  SetVel(vel);
//std::cout << "Vel: " << m_vel.x << ", " << m_vel.y << ", " << m_vel.z << "\n";

  GameObject::Update();
    
  Vec3f dir = GetPos() - m_newPos;
  if (dir.SqLen() < 1.0f) // TODO CONFIG
  {
    SetVel(Vec3f(0, 0, 0));
    m_newPos = GetPos();
  }

  float speed = m_vel.SqLen();

  static const float MAX_SPEED = 100.0f; // TODO CONFIG
  static const float RUN_SPEED = MAX_SPEED * 0.5f;
  static const float WALK_SPEED = RUN_SPEED * 0.5f;
  
  if (speed > RUN_SPEED)
  {
    m_sceneNode->SetAnim("run");
  }
  else if (speed > WALK_SPEED)
  {
    m_sceneNode->SetAnim("walk");
  }
  else
  {
    m_sceneNode->SetAnim("stand");
  }
}

bool Player::UpdateState(PXml xml) 
{
  // TODO
  return true;
}
}

