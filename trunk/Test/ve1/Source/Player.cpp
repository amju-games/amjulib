#include "Player.h"
#include <AmjuGL.h>
#include <Teapot.h>
#include "Ve1SceneGraph.h"
#include <iostream>
#include "Ve1Character.h"
#include <File.h>
#include "Useful.h"
#include "AvatarManager.h"
#include <StringUtils.h>
#include <DegRad.h>
#include <Timer.h>
#include <GuiText.h>
#include <DrawAABB.h>
#include "LocalPlayer.h"
#include "GSMain.h"
#include <GameObjectFactory.h>

namespace Amju
{
class PlayerSceneNode : public Ve1Character
{
public:
  PlayerSceneNode(Player* p) : m_player(p)
  {
  }

  virtual void Update()
  {
    if (GetMd2())
    {
      Ve1Character::Update();
    }
  }

  virtual void Draw()
  {
    if (GetMd2())
    {
      Ve1Character::Draw();

      Assert(m_player->GetAABB());
      DrawAABB(*(m_player->GetAABB()));
    }

    // TODO Print name 
    /*
    GuiText text;
    text.SetText(m_player->GetName());
    text.SetSize(Vec2f(20.0f, 20.0f));
    AmjuGL::PushMatrix();
//    AmjuGL::Translate(0, 10, 0);
//    AmjuGL::Scale(20, 20, 20);
    text.Draw();
    AmjuGL::PopMatrix();
    */
  }

protected:
  RCPtr<Player> m_player;
};

const char* Player::TYPENAME = "player";


GameObject* CreatePlayer()
{
  return new Player;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Player::TYPENAME, CreatePlayer);

Player::Player() : m_posHasBeenSet(false), m_sceneNode(0)
{
  m_dir = 0;
  m_dirCurrent = m_dir;
  m_isLocal = false;
}

const std::string& Player::GetName() const
{
  return m_name;
}

void Player::SetName(const std::string& name)
{
  m_name = name;
}

bool Player::IsLocalPlayer() const
{
  return m_isLocal;
}
     
void Player::SetIsLocalPlayer(bool isLocal)
{
  m_isLocal = isLocal;
}

bool Player::Load(File* f)
{
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);

  PlayerSceneNode* psn = new PlayerSceneNode(this);
  m_sceneNode = psn;

/*  
  // Avatar set using Set() below

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
*/

  root->AddChild(psn);

#ifdef PLAYER_LOAD_DEBUG
std::cout << "Loaded player ok!\n";
#endif

  return true;
}

void Player::Draw()
{
  Assert(GetAABB());
  DrawAABB(*GetAABB());
}

void Player::Set(const std::string& key, const std::string& val)
{
  Ve1Object::Set(key, val);
  if (key == "type")
  {
    int type = ToInt(val); // TODO overload Set to take an int
    TheAvatarManager::Instance()->SetAvatar(type, m_sceneNode);
  }
  else if (key == "tex")
  {
    int texNum = ToInt(val);
    TheAvatarManager::Instance()->SetTexture(texNum, m_sceneNode);
  }
  else if (key == "name")
  {
    SetName(val);
  }
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

      // Work out direction to face
      SetDir(RadToDeg(atan2((double)m_vel.x, (double)m_vel.z)));
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

  static const float XSIZE = 15.0f;
  static const float YSIZE = 60.0f;
  GetAABB()->Set(
    m_pos.x - XSIZE, m_pos.x + XSIZE,
    m_pos.y, m_pos.y + YSIZE,
    m_pos.z - XSIZE, m_pos.z + XSIZE);

  GameObject::Update();
    
  Vec3f dir = GetPos() - m_newPos;
  if (dir.SqLen() < 1.0f) // TODO CONFIG
  {
    SetVel(Vec3f(0, 0, 0));
    m_newPos = GetPos();
  }

  TurnToFaceDir();

  if (m_sceneNode->GetMd2())
  {
    float speed = m_vel.SqLen();

    static const float MAX_SPEED = 100.0f; // TODO CONFIG
    static const float RUN_SPEED = MAX_SPEED * 0.5f;
    static const float WALK_SPEED = RUN_SPEED * 0.5f;
  
    if (speed > RUN_SPEED)
    {
      m_sceneNode->SetAnim("walk"); //"run");
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
}

void Player::SetDir(float degs)
{
  m_dir = degs;
}

void Player::TurnToFaceDir()
{
  Matrix mat;
  mat.RotateY(DegToRad(m_dirCurrent));
  mat.TranslateKeepRotation(m_pos);
  m_sceneNode->SetLocalTransform(mat);

  static const float ROT_SPEED = 10.0f; // TODO CONFIG
  float dt = TheTimer::Instance()->GetDt();
  float angleDiff = m_dir - m_dirCurrent;

  // Rotate to face m_dir, taking the shortest route (CW or CCW)
  if (fabs(angleDiff) < 10.0f)
  {
    m_dirCurrent = m_dir;
  }
  else
  {
    if (angleDiff < -180.0f)
    {
      m_dir += 360.0f;
    }
    else if (angleDiff > 180.0f)
    {
      m_dir -= 360.0f;
    }
    angleDiff = m_dir - m_dirCurrent;

    if (m_dirCurrent > m_dir)
    {
      m_dirCurrent -= ROT_SPEED * dt * fabs(angleDiff);
    }
    else if (m_dirCurrent < m_dir)
    {
      m_dirCurrent += ROT_SPEED * dt * fabs(angleDiff);
    }
  }
}

AABB* Player::GetAABB()
{
  return m_sceneNode->GetAABB();
}

class CommandTalk : public GuiCommand
{
public:
  CommandTalk(Player* p) : m_player(p) {}

  virtual bool Do()
  {
    TheGSMain::Instance()->ActivateChatSend(true, m_player->GetId());
    return false;
  }

private:
  Player* m_player;
};

void Player::SetMenu(GuiMenu* menu)
{
  if (IsLocalPlayer())
  {
  }
  else
  {
    menu->AddItem(new GuiMenuItem("Talk to this player", new CommandTalk(this)));
  }
}
}

