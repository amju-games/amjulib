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
#include <Shadow.h>
#include "Terrain.h"
#include <CollisionMesh.h>
#include "Useful.h"

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
 
      // Print name 
      // TODO Do all these in one go, to minimise state changes
      AmjuGL::Enable(AmjuGL::AMJU_BLEND);
      AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

      GuiText text;
      text.SetTextSize(10.0f);
      text.SetText(m_player->GetName());
      text.SetSize(Vec2f(10.0f, 1.0f));
      text.SetJust(GuiText::AMJU_JUST_LEFT);
      //text.SetInverse(true);
      //text.SetDrawBg(true);

      AmjuGL::PushMatrix();
    
      Matrix m;
      m.SetIdentity();
      Vec3f tr(m_local[12], m_local[13], m_local[14]);
      m.Translate(tr);
      AmjuGL::MultMatrix(m);
      AmjuGL::Translate(0, 70.0f, 0);
    
      AmjuGL::Scale(20, 20, 10);  

      text.Draw();
      AmjuGL::PopMatrix();

      AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
      AmjuGL::Disable(AmjuGL::AMJU_BLEND);
    }
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
  m_isMoving = false;
  m_inNewLocation = false;
}

const std::string& Player::GetName() const
{
  return m_name;
}

void Player::SetName(const std::string& name)
{
  m_name = name;
}

//bool Player::IsLocalPlayer() const
//{
//  return m_isLocal;
//}
     
//void Player::SetIsLocalPlayer(bool isLocal)
//{
//  m_isLocal = isLocal;
//}

bool Player::Load(File* f)
{
  // Create Scene Node, but don't attach to SceneGraph until needed  
  PlayerSceneNode* psn = new PlayerSceneNode(this);
  m_sceneNode = psn;

  m_shadow = new Shadow;
  if (!m_shadow->Load(f))
  {
    return false;
  }
  m_sceneNode->AddChild(m_shadow.GetPtr());

  // Load arrow scene node
  ObjMesh* arrowMesh = (ObjMesh*)TheResourceManager::Instance()->GetRes("arrow.obj"); 
  Assert(arrowMesh);
  m_arrow = new SceneMesh;
  m_arrow->SetMesh(arrowMesh);
//  m_arrow->UpdateBoundingVol();
//  m_sceneNode->AddChild(m_arrow.GetPtr()); // TODO TEMP TEST

  return true;
}

void Player::OnLocationExit()
{
  // We can't be the Local Player - this function is called when an object leaves the location
  //  where the local player is :-)
  Assert(!IsLocalPlayer());

  // Remove from SceneGraph
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);
  root->DelChild(m_sceneNode.GetPtr());
  root->DelChild(m_arrow.GetPtr());
}

void Player::OnLocationEntry()
{
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);

std::cout << "Adding scene node to SceneGraph for player\n";

  root->AddChild(m_sceneNode.GetPtr());
  root->AddChild(m_arrow.GetPtr());
  SetArrowVis(false);

  m_isMoving = false;
  m_inNewLocation = true;
}

// Scene Graph

//void Player::Draw()
//{
//  Assert(GetAABB());
//  DrawAABB(*GetAABB());
//}

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

void Player::SetArrowVis(bool visible)
{
std::cout << "Setting arrow vis to " << (visible ? "true" : "false") << "\n";

  m_arrow->SetVisible(visible);
}

void Player::SetArrowPos(const Vec3f& newpos)
{
std::cout << " Setting arrow pos to " << newpos << "\n";

  Matrix m;
  m.Translate(newpos);
  m_arrow->SetLocalTransform(m);

  static const float XSIZE = 15.0f;
  static const float YSIZE = 60.0f;
  m_arrow->GetAABB()->Set(
    newpos.x - XSIZE, newpos.x + XSIZE,
    newpos.y, newpos.y + YSIZE,
    newpos.z - XSIZE, newpos.z + XSIZE);
}

void Player::MoveTo(const Vec3f& newpos)
{
  m_newPos = newpos;
  m_isMoving = true;

  // TODO Change position immediately if this is first MoveTo() for this location.
  // Otherwise we should walk to the new pos.

  if (m_posHasBeenSet)
  {
std::cout << "Player: got new pos to move to: " << newpos << ", current pos is " << GetPos() << "\n";

    Vec3f dir = GetPos() - newpos;

    /*
    if (dir.SqLen() < 1.0f) // TODO CONFIG
    {
      SetVel(Vec3f(0, 0, 0));
      SetArrowVis(false);
    }
    else
    */

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
std::cout << "Player: first pos update, so set immediately\n";

    // First pos update, so immediately set pos
    m_posHasBeenSet = true;
    SetPos(newpos); 
    m_isMoving = false;
  }
}

void Player::Update()
{
  // Not safe to do anything if the Terrain has not been created yet 
  if (!TerrainReady())
  {
    return; // Don't do nuffink 
  }

  GameObject::Update();
  
  // Tell shadow the collision mesh it is casting onto
  // TODO Use octree etc
  // NB This only works if Terrain is activated before player!!
  // Also this crashes if Terrain not created at all yet!!
  // -> General problem of relying on another object already being in existence: you can't!!
  // When you start, and when you change locations, you must wait for the Terrain to be set up.
  // Otherwise we will be falling through floor, etc. if not crashing.

  // Just do once per location, but only when Terrain is valid.
  // I.e. have a flag, set it in OnLocationEntry. It's not complicated.

  if (m_inNewLocation)
  {
    m_shadow->ClearCollisionMeshes(); 
    m_shadow->AddCollisionMesh(GetTerrain()->GetCollisionMesh());
    m_inNewLocation = false;
  }

  // Get height for (x, z);
  float y = 0;
  if (GetTerrain()->GetCollisionMesh()->GetY(Vec2f(m_pos.x, m_pos.z), &y))
  {
    m_pos.y = y;
  }

  // Stop moving if we are close enough to the destination
  // TODO This ends up happening every frame, only do it if we are moving
  if (m_isMoving)
  {
    Vec3f dir = GetPos() - m_newPos;
    dir.y = 0; // ignore y coord for now
    if (dir.SqLen() < 1.0f) // TODO CONFIG
    {
      SetVel(Vec3f(0, 0, 0));
      m_newPos = GetPos();
      SetArrowVis(false);
      m_isMoving = false;
    }
  }

  if (m_sceneNode)
  {
    Matrix m;
    m.Translate(m_pos);
    m_sceneNode->SetLocalTransform(m);
    m_sceneNode->Update();

    // Set shadow AABB to same as Scene Node so we don't cull it by mistake
    *(m_shadow->GetAABB()) = *(m_sceneNode->GetAABB());

    // TODO TEMP TEST
////    *(m_arrow->GetAABB()) = *(m_sceneNode->GetAABB());

    static const float XSIZE = 15.0f;
    static const float YSIZE = 60.0f;
    GetAABB()->Set(
      m_pos.x - XSIZE, m_pos.x + XSIZE,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - XSIZE, m_pos.z + XSIZE);

    TurnToFaceDir();

    if (m_sceneNode->GetMd2())
    {
      float speed = m_vel.SqLen();

      // TODO Simplify -- either moving or idle. 
      // NB Speeds should be an avatar variable and level up

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

