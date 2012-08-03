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
#include "ObjectUpdater.h"

namespace Amju
{
static const float ARROW_XSIZE = 5.0f;
static const float ARROW_YSIZE = 30.0f;

class PlayerSceneNode : public Ve1Character
{
public:
  PlayerSceneNode(Player* p) : Ve1Character(p), m_player(p)
  {
  }

  virtual void Update()
  {
    if (!IsVisible()) 
    {
      return;
    }

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
    if (IsVisible() && GetMd2() && m_player)
    {
      bool loggedIn = m_player->IsLoggedIn();
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
    if (IsVisible())
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
  m_carrying = 0;
}

void Player::SetCarrying(Ve1Object* obj)
{
  if (obj)
  {
    if (m_carrying)
    {
//std::cout << "Must drop current object first!\n";
    }
    else
    {
std::cout << *this << " is now carrying " << *obj << "\n";
    }
  }
  else
  {
    if (m_carrying)
    {
std::cout << "Dropping currently carried object " << *m_carrying << ".\n";
    }
    else
    {
std::cout << "?? Got drop object by not carrying anything\n";
    }
  }

  m_carrying = obj;
}

Ve1Object* Player::GetCarrying()
{
  return m_carrying;
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

  return true;
}

void Player::OnLocationExit()
{
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

  root->AddChild(m_sceneNode.GetPtr());
  root->AddChild(m_arrow.GetPtr());
  SetArrowVis(false);

  // TODO Portal should have a heading which you should face when you appear at the destination
  m_isMoving = false; 
  SetVel(Vec3f(0, 0, 0)); // TODO walk out of doorway ?

  // TODO Set m_newPos ??

  m_inNewLocation = true;

  // Set appearance
  SetLoggedIn(IsLoggedIn());

  // If we are carrying something, change its location too
  if (m_carrying)
  {
    TheObjectUpdater::Instance()->SendChangeLocationReq(m_carrying->GetId(), GetPos(), GetLocation()); 
  }
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
  Ve1ObjectChar::SetKeyVal(key, val);
  
  static ChatConsole* cc = TheChatConsole::Instance();

  if (key == "loggedin")
  {
    bool isLoggedIn = (val == "y");
    SetLoggedIn(isLoggedIn);
    SetLoggedInPlayer(this, m_isLoggedIn);
    static GSMain* gsm = TheGSMain::Instance();
    gsm->SetNumPlayersOnline(CountOnlinePlayers());

    // So we can notify if current recipient logs in or out
    cc->SetPlayerLoggedIn(this, isLoggedIn);
  }
  else if (key == "istyping")
  {
    int recipId = ToInt(val);
    bool isTyping = (recipId > 0);
    cc->SetPlayerIsTyping(isTyping, GetId(), recipId); 
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

void Player::SetLoggedIn(bool loggedIn)
{
  m_isLoggedIn = loggedIn;

  Assert(m_sceneNode);

  // Translucent if logged out
  // Does this have to be every frame.
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
    m_sceneNode->SetBlended(true); // problems ?
  }
}

void Player::Update()
{
  Ve1ObjectChar::Update();

  if (m_hidden)
  {
    return;
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
  else
  {
    //Assert(GetVel().SqLen() == 0);
  }

  if (m_sceneNode)
  {
    // Set shadow AABB to same as Scene Node so we don't cull it by mistake
    *(m_nameTag->GetAABB()) = *(m_sceneNode->GetAABB());
  }

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
  if (p)
  {
    *r = p->GetName();
    return true;
  }
  return false;
}
}

