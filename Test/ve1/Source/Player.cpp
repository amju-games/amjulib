#include "Player.h"
#include <Game.h>
#include <AmjuGL.h>
//#include <Teapot.h>
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
#include "ChatConsole.h"

namespace Amju
{
static const float ARROW_XSIZE = 5.0f;
static const float ARROW_YSIZE = 30.0f;

class PlayerSceneNode : public Ve1Character
{
public:
  PlayerSceneNode(Player* p) : m_player(p)
  {
  }

  virtual void Update()
  {
    if (GetMd2() && m_player && m_player->IsLoggedIn())
    {
      Ve1Character::Update();
    }
  }

/*
  // TODO This doesn't work - SceneGraph is broken, sigh.

  virtual void BeforeDraw()
  {
    bool loggedin = m_player->IsLoggedIn();
    if (loggedin)
    {
      PushColour();
      MultColour(Colour(0.2, 0.2, 0.2, 0.2f));
    }
  }

  virtual void AfterDraw()
  {
    bool loggedin = m_player->IsLoggedIn();
    if (loggedin)
    {
      PopColour();
    }
  }
*/

  virtual void Draw()
  {
    bool loggedIn = m_player->IsLoggedIn();
    if (GetMd2() && m_player)
    {
      if (!loggedIn)
      {
        AmjuGL::PushAttrib(AmjuGL::AMJU_BLEND);
        AmjuGL::Enable(AmjuGL::AMJU_BLEND);
      }

      Ve1Character::Draw();

      if (!loggedIn)
      {
        AmjuGL::PopAttrib();
      }
    }
  }
 
protected:
  RCPtr<Player> m_player;
};

  // TODO Name tag scene node
class PlayerNameNode : public SceneNode
{
public:
  PlayerNameNode(Player* p) : m_player(p)
  {
    SetBlended(true);
  }

  virtual void Draw()
  {
      //Assert(m_player->GetAABB());
      //DrawAABB(*(m_player->GetAABB()));
 
      // Print name 
      // TODO Do all these in one go, to minimise state changes
      AmjuGL::PushAttrib(AmjuGL::AMJU_BLEND | AmjuGL::AMJU_DEPTH_READ);
      AmjuGL::Enable(AmjuGL::AMJU_BLEND);
      AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

      GuiText text;
      text.SetTextSize(5.0f); // TODO CONFIG
      text.SetText(m_player->GetName());
    
      static const float MAX_NAME_WIDTH = 4.0f; // minimise this to reduce overdraw - calc from text
      text.SetSize(Vec2f(MAX_NAME_WIDTH, 1.0f));
      text.SetJust(GuiText::AMJU_JUST_CENTRE);
      //text.SetInverse(true);
      //text.SetDrawBg(true);

      AmjuGL::PushMatrix();
    
      Matrix m;
      m.SetIdentity();
      //Vec3f tr(m_local[12], m_local[13], m_local[14]);
      Vec3f tr(m_combined[12], m_combined[13], m_combined[14]);
      m.Translate(tr);
      AmjuGL::MultMatrix(m);
      static const float SCALE_FACTOR = 20.0f;
      float x = MAX_NAME_WIDTH * SCALE_FACTOR * -0.5f;
      AmjuGL::Translate(x, 60.0f, 0); // TODO CONFIG
    
      AmjuGL::Scale(SCALE_FACTOR, SCALE_FACTOR, 10);  

      text.Draw();
      AmjuGL::PopMatrix();
      AmjuGL::PopAttrib();
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

Player::Player() 
{
  m_isLocal = false;
  m_isLoggedIn = false;
  m_fadeTime = 0;
}

bool Player::IsLoggedIn() const
{
  return m_isLoggedIn;
}

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

  m_nameTag = new PlayerNameNode(this);
  m_sceneNode->AddChild(m_nameTag.GetPtr());

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
  //////Assert(!IsLocalPlayer());

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

//std::cout << "Adding scene node to SceneGraph for player\n";

  root->AddChild(m_sceneNode.GetPtr());
  root->AddChild(m_arrow.GetPtr());
  SetArrowVis(false);

  // TODO Portal should have a heading which you should face when you appear at the destination
  m_isMoving = false; 
  SetVel(Vec3f(0, 0, 0)); // TODO walk out of doorway ?

  // TODO Set m_newPos ??

  m_inNewLocation = true;
}

typedef std::map<Player*, bool> LoggedInMap;
LoggedInMap loggedInMap;

static void SetLoggedInPlayer(Player* p, bool b)
{
  loggedInMap[p] = b;
}

static int CountOnlinePlayers()
{
  int res = 0;
  for (LoggedInMap::iterator it = loggedInMap.begin(); it != loggedInMap.end(); ++it)
  {
    if (it->second)
    {
      res++;
    }
  }
  return res;
}

void Player::SetKeyVal(const std::string& key, const std::string& val)
{
  Ve1Object::SetKeyVal(key, val);
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
  else if (key == "loggedin")
  {
    m_isLoggedIn = (val == "y");
    SetLoggedInPlayer(this, m_isLoggedIn);
    static GSMain* gsm = TheGSMain::Instance();
    gsm->SetNumPlayersOnline(CountOnlinePlayers());
  }
  else if (key == "istyping")
  {
    int recipId = ToInt(val);
    bool isTyping = (recipId > 0);
    TheChatConsole::Instance()->SetPlayerIsTyping(isTyping, GetId(), recipId); 
  }
}

void Player::SetArrowVis(bool visible)
{
//std::cout << "Setting arrow vis to " << (visible ? "true" : "false") << "\n";

  m_arrow->SetVisible(visible);
}

void Player::SetArrowPos(const Vec3f& newpos)
{
//std::cout << " Setting arrow pos to " << newpos << "\n";

  Matrix m;
  m.Translate(newpos);
  m_arrow->SetLocalTransform(m);

  m_arrow->GetAABB()->Set(
    newpos.x - ARROW_XSIZE, newpos.x + ARROW_XSIZE,
    newpos.y, newpos.y + ARROW_YSIZE,
    newpos.z - ARROW_XSIZE, newpos.z + ARROW_XSIZE);
}

void Player::Update()
{
  Ve1ObjectChar::Update();

  // Translucent if logged out
  if (IsLoggedIn())
  {
    m_sceneNode->SetColour(Colour(1, 1, 1, 1));
    m_sceneNode->SetBlended(false);
  }
  else
  {
    // TODO Translucent has odd effects, players lose their translucency depending on camera pos ????
    static const float GREY = 0.0f;
    m_sceneNode->SetColour(Colour(GREY, GREY, GREY, 0.2f));
    m_sceneNode->SetBlended(false); // problems
  }
  
/*
  // Not safe to do anything if the Terrain has not been created yet 
  if (!TerrainReady())
  {
    return; 
  }

  ////////GameObject::Update(); // TODO Why not Ve1Object ?
  
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
  // Get closest Y value to current, not the highest or lowest
  if (GetTerrain()->GetCollisionMesh()->GetClosestY(Vec2f(m_pos.x, m_pos.z), m_pos.y, &y))
  {
    m_pos.y = y;
  }
*/

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
  else
  {
    Assert(GetVel().SqLen() == 0);
  }

  if (m_sceneNode)
  {
    // Set shadow AABB to same as Scene Node so we don't cull it by mistake
    *(m_nameTag->GetAABB()) = *(m_sceneNode->GetAABB());
  }

/*
  if (m_sceneNode)
  {
    Matrix m;
    m.Translate(m_pos);
    m_sceneNode->SetLocalTransform(m);
    //m_sceneNode->Update(); // done for whole scene graph elsewhere

    // Set shadow AABB to same as Scene Node so we don't cull it by mistake
    *(m_shadow->GetAABB()) = *(m_sceneNode->GetAABB());

    // TODO TEMP TEST
////    *(m_arrow->GetAABB()) = *(m_sceneNode->GetAABB());

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
*/

  if (m_ignorePortalId != -1)
  {
    GameObject* g = TheGame::Instance()->GetGameObject(m_ignorePortalId);
    if (g)
    {
      AABB* aabb = g->GetAABB();
      if (aabb)
      {
        if (!GetAABB()->Intersects(*aabb))
        {
          // No longer intersecting portal
          m_ignorePortalId = -1;
        }
      }
      else
      {
        m_ignorePortalId = -1; // why ?
      }
    }
    else
    {
      m_ignorePortalId = -1; // ?
    }

  }
}

class CommandTalk : public GuiCommand
{
public:
  CommandTalk(Player* p) : m_player(p) {}

  virtual bool Do()
  {
    TheChatConsole::Instance()->ActivateChatSend(true, m_player->GetId());
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
  else if (IsLoggedIn())
  {
    menu->AddChild(new GuiMenuItem("Talk to " + GetName(), new CommandTalk(this)));
  }
  else
  {
    menu->AddChild(new GuiMenuItem("Leave a message for " + GetName(), new CommandTalk(this)));
  }
}

bool GetNameForPlayer(int objId, std::string* r)
{
  Player* p = dynamic_cast<Player*>(TheGame::Instance()->GetGameObject(objId).GetPtr());
  //Assert(p);
  if (p)
  {
    *r = p->GetName();
    return true;
  }
  return false;
}
}

