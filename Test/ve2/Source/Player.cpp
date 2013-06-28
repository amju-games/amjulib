#include <AmjuFirst.h>
#include "Player.h"
#include <Game.h>
#include <AmjuGL.h>
#include <SoundManager.h>
#include "Ve1SceneGraph.h"
#include <iostream>
#include "Ve1Character.h"
#include <File.h>
#include "Useful.h"
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
#include "AttackEffect.h"
#include "CommandFight.h"
#include "LurkMsg.h"
#include "GameConsts.h"
#include "ROConfig.h"
#include "HeartCount.h"
#include "CommandPickup.h"
#include "FirstTimeMsg.h"
#include "MsgNum.h"
#include "GameLookup.h"
#include "Achievement.h"
#include "ObjectManager.h"
#include "FoodCount.h"
#include "PlayerNames.h"
#include "Ve1SpriteNode.h"
#include "Food.h"
#include "GS2dAvatarMod.h"
#include "MsgManager.h"
#include "GameMode.h"
#include "GameConsts.h"
#include <AmjuFinal.h>

namespace Amju
{
//static const float ARROW_XSIZE = 5.0f;
//static const float ARROW_YSIZE = 30.0f;

// Name tag scene node
class PlayerNameNode : public SceneNode
{
public:
  PlayerNameNode(Player* p) : m_player(p)
  {
    SetBlended(true);
    m_text.SetTextSize(5.0f); // TODO CONFIG
    static const float MAX_NAME_WIDTH = 4.0f; // minimise this to reduce overdraw - calc from text
    m_text.SetSize(Vec2f(MAX_NAME_WIDTH, 1.0f));
    m_text.SetJust(GuiText::AMJU_JUST_CENTRE);
    m_text.SetFgCol(Colour(0, 0, 0, 1));
    //m_text.SetInverse(true);
    //m_text.SetDrawBg(true);
  }

  virtual void Draw()
  {
    Assert(m_player);
    if (GetGameMode() == AMJU_MODE_SINGLE)
    {
      // Don't draw non-local player
      if (!m_player->IsLocalPlayer())
      {
        return;
      }
    }

    // Don't draw name of local player ?
    if (IsVisible())  // && m_player->GetId() != GetLocalPlayerId())
    {
      //Assert(m_player->GetAABB());
      //DrawAABB(*(m_player->GetAABB()));
  
      std::string name = m_player->GetName();
      if (!m_player->IsLoggedIn())
      {
        name += " (asleep)";
      }
      m_text.SetText(name);    
 
      // Print name 
      // TODO Do all these in one go, to minimise state changes
      AmjuGL::PushAttrib(AmjuGL::AMJU_BLEND | AmjuGL::AMJU_DEPTH_READ);
      AmjuGL::Enable(AmjuGL::AMJU_BLEND);
      AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

      AmjuGL::PushMatrix();
    
      Matrix m;
      m.SetIdentity();

      // Reverse modelview rotation
      Matrix r;
      r.ModelView();
      m = TransposeRot(r);

      Vec3f tr(m_combined[12], m_combined[13], m_combined[14]);
      m.TranslateKeepRotation(tr);
      AmjuGL::MultMatrix(m);
      static const float SCALE_FACTOR = 20.0f;
      static const float MAX_NAME_WIDTH = 4.0f; // minimise this to reduce overdraw - calc from text
      float x = MAX_NAME_WIDTH * SCALE_FACTOR * -0.5f;
      AmjuGL::Translate(x, 60.0f, 0); // TODO CONFIG
    
      AmjuGL::Scale(SCALE_FACTOR, SCALE_FACTOR, 10);  

      m_text.Draw();
      AmjuGL::PopMatrix();
      AmjuGL::PopAttrib();
    }
  }
 
protected:
  RCPtr<Player> m_player;
  GuiText m_text;
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

  m_lastFoodCount = -1;
  m_isDead = false;
  m_totalFoods = -1; // not set yet

  Ve1SpriteNode* sn = new Ve1SpriteNode;
  sn->SetObj(this);
  SetSceneNode(sn);
}

LayerSprite& Player::GetSprite() 
{
  Assert(m_sceneNode);
  return ((Ve1SpriteNode*)m_sceneNode.GetPtr())->GetSprite();
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

void Player::SetDir(float degs)
{
  Ve1ObjectChar::SetDir(degs);

  // Makes AABBs inaccurate for long objects etc
#ifdef ALLOW_ARBITRARY_ANGLES
  if (m_carrying)
  {
    m_carrying->SetDir(degs);
  }
#endif
}

bool Player::IsLoggedIn() const
{
  return m_isLoggedIn;
}

bool Player::Load(File* f)
{
  return true; 



  // Create Scene Node, but don't attach to SceneGraph until needed  

  // TODO We can't until we call SetFromCharacterName in SetKeyVal

  //PlayerSceneNode* psn = new PlayerSceneNode(this);
  //m_sceneNode = psn;

  if (!m_shadow->Load(f))
  {
    return false;
  }

  // Load arrow scene node
/*
  ObjMesh* arrowMesh = (ObjMesh*)TheResourceManager::Instance()->GetRes("arrow.obj"); 
  Assert(arrowMesh);
  m_arrow = new SceneMesh;
  m_arrow->SetMesh(arrowMesh);
*/

  // Particle effect when attacked, etc.
  File fight;
  if (!fight.OpenRead("fighteffect.txt"))
  {
    fight.ReportError("Failed to load effect");
    return false;
  }

  m_effect = new AttackEffect;
  if (!m_effect->Load(&fight))
  {
    fight.ReportError("Failed to load effect");
    return false;
  }
  m_effect->SetVisible(true);

  return true;
}

void Player::OnLocationExit()
{
  // Remove from SceneGraph
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);
  root->DelChild(m_sceneNode.GetPtr());
  //root->DelChild(m_arrow.GetPtr());
}

void Player::OnLocationEntry()
{
  Ve1ObjectChar::OnLocationEntry();

/*
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  if (root)
  {
    root->AddChild(m_arrow.GetPtr());
    SetArrowVis(false);
  }
*/

  m_nameTag = new PlayerNameNode(this);
  if (m_sceneNode)
  {
    if (m_nameTag)
    {
      m_sceneNode->AddChild(m_nameTag.GetPtr());
    }

    if (m_effect)
    {
      m_sceneNode->AddChild(m_effect.GetPtr());
    }
  }

  // TODO Portal should have a heading which you should face when you appear at the destination
  m_isMoving = false; 
  SetVel(Vec3f(0, 0, 0)); // TODO walk out of doorway ?

  // TODO Set m_newPos ??

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
  static GSMain* gsm = TheGSMain::Instance();

  if (key == "loggedin")
  {
    bool isLoggedIn = (val == "y");
    SetLoggedIn(isLoggedIn);
    SetLoggedInPlayer(this, m_isLoggedIn);
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
/*
  else if (key == "beat_up_by")
  {
    if (val != "0" && IsLocalPlayer())
    {
      TheObjectUpdater::Instance()->SendUpdateReq(GetId(), "beat_up_by", "0");
      int oppId = ToInt(val);
      static int prevOpp = 0;
      Player* opp = dynamic_cast<Player*>(TheGame::Instance()->GetGameObject(oppId).GetPtr());
      std::string s = "You just got beat up!";
      if (opp && !opp->GetName().empty())
      {
        s = "You just got beat up by " + opp->GetName();
        if (oppId == prevOpp)
        {
          s += " again!";
        }
        else
        {
          s += "!";
        }
      }
      prevOpp = oppId; // remember previous opponent

      LurkMsg lm(s, LURK_FG, LURK_BG, AMJU_CENTRE); 
      TheLurker::Instance()->Queue(lm);
      ShowAttacked();
    }
  }
*/
/*
  else if (key == FOOD_KEY)
  {
    if (IsLocalPlayer())
    {
      int fc = ToInt(val);
      // Lurk msg - total number of Food cells ever brought to ship
      std::string s;
      if (m_totalFoods != -1)
      {
        if (fc == 1)
        {
          s = "You brought a Food cell to the ship!";
        }
        else
        {
          s = "You have brought a total of " + val + " Food cells to the ship!";
        }
        LurkMsg lm(s, LURK_FG, LURK_BG, AMJU_CENTRE); 
        TheLurker::Instance()->Queue(lm);
      }
      m_totalFoods = fc;

      if (fc > 0 && !HasWonAchievement(ACH_FOOD_CELL_TO_SHIP_1))
      {
        OnWinAchievement(ACH_FOOD_CELL_TO_SHIP_1, "You brought your first Food cell back to the ship!");
      }
      if (fc >= 5 && !HasWonAchievement(ACH_FOOD_CELL_TO_SHIP_5))
      {
        OnWinAchievement(ACH_FOOD_CELL_TO_SHIP_5, "You brought 5 Food cells back to the ship!");
      }
      if (fc >= 10 && !HasWonAchievement(ACH_FOOD_CELL_TO_SHIP_10))
      {
        OnWinAchievement(ACH_FOOD_CELL_TO_SHIP_10, "You brought 10 Food cells back to the ship!");
      }
      // etc
    }
  }
*/
  else if (key == LAST_MSG_SENT_KEY)
  {
    TheMsgManager::Instance()->CheckForNewMsgs();
  }
}

void Player::SetArrowVis(bool visible)
{
/*
  m_arrow->SetVisible(visible);
*/
}

void Player::SetArrowPos(const Vec3f& newpos)
{
/*
//std::cout << " Setting arrow pos to " << newpos << "\n";

  Matrix m;
  m.Translate(newpos);
  m_arrow->SetLocalTransform(m);

  m_arrow->GetAABB()->Set(
    newpos.x - ARROW_XSIZE, newpos.x + ARROW_XSIZE,
    newpos.y, newpos.y + ARROW_YSIZE,
    newpos.z - ARROW_XSIZE, newpos.z + ARROW_XSIZE);
*/
}

void Player::SetLoggedIn(bool loggedIn)
{
  m_isLoggedIn = loggedIn;

  //Assert(m_sceneNode);

  /*
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
  */
}

void Player::Update()
{
  if (m_isLoggedIn)
  {
    Ve1ObjectChar::Update();
  }
  else if (m_sceneNode)
  {
    Matrix m;
    m.Translate(m_pos);
    m_sceneNode->SetLocalTransform(m);

    // Set shadow AABB to same as Scene Node so we don't cull it by mistake
    m_shadow->SetAABB(*(m_sceneNode->GetAABB()));

    static const float XSIZE = ROConfig()->GetFloat("player-aabb-x", 40.0f);
    static const float YSIZE = ROConfig()->GetFloat("player-aabb-y", 10.0f);
    static const float ZSIZE = ROConfig()->GetFloat("player-aabb-z", 40.0f);

    GetAABB()->Set(
      m_pos.x - XSIZE, m_pos.x + XSIZE,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - ZSIZE, m_pos.z + ZSIZE);

    /*
    DISABLED for 2D look and feel
    TurnToFaceDir();
    */
  }

  if (m_hidden)
  {
    return;
  }

  // Stop moving if we are close enough to the destination
  // TODO This ends up happening every frame, only do it if we are moving
  if (true) //m_isMoving)
  {
    Vec3f dir = GetPos() - m_newPos;
    dir.y = 0; // ignore y coord for now
    static const float STOP_DISTANCE = ROConfig()->GetFloat("stop-dist", 20.0f);
    if (dir.SqLen() < STOP_DISTANCE) 
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

  static GSMain* gsm = TheGSMain::Instance();
  if (IsLocalPlayer())
  {
    GameMode gm = GetGameMode();
    if (gm == AMJU_MODE_SINGLE)
    {
      // Score is food eaten
      gsm->SetScore(m_stamina); // TODO
    }
    else if (gm == AMJU_MODE_MULTI)
    {
      // Score is food given + food recv ??
      gsm->SetScore(m_stamina); // TODO
    }

//    TheGSMain::Instance()->SetFoods(GetFoodRecvCount());

/*
    if (m_stamina <= 0)
    {
      // Player now has to go back to the spaceship to regenerate or something.
      if (!m_isDead)
      {
        LurkMsg lm("You need to return to your spaceship to recover from your injuries!", 
          Colour(1, 1, 1, 1), Colour(1, 0, 0, 1), AMJU_CENTRE); 
        TheLurker::Instance()->Queue(lm);
      }

      m_isDead = true;
    }
*/
  }
}

class CommandAvatarMod : public GuiCommand
{
public:
  virtual bool Do()
  {
    static GS2dAvatarMod* gs = TheGS2dAvatarMod::Instance();
    gs->SetPrevState(TheGSMain::Instance());
    TheGame::Instance()->SetCurrentState(gs);
    return true;
  }
};

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
    AddMenuItem("Change look", new CommandAvatarMod);

    // If carrying, drop is option
    if (m_carrying)
    {
      AddMenuItem("Put down", new CommandPickUp(m_carrying, false));
    }
  }
  else if (IsLoggedIn())
  {
    //menu->AddChild(new GuiMenuItem("Talk to " + GetName(), new CommandTalk(this)));

// No one-to-one chat
//    AddMenuItem("Talk to " + GetName(), new CommandTalk(this));
    /*
    NO FIGHTING
    AddMenuItem("FIGHT!!", new CommandFight(this));
    */
  }
  else
  {
    // TODO put back leaving messages
    //menu->AddChild(new GuiMenuItem("Leave a message for " + GetName(), new CommandTalk(this)));

// No leaving messages
//    AddMenuItem("Leave a message for " + GetName(), new CommandTalk(this));
  }
}

/*
int Player::GetFoodRecvCount()
{
  if (Exists(FOOD_RECEIVED_KEY))
  {
    std::string s = GetVal(FOOD_RECEIVED_KEY);
    int i = ToInt(s);
    return i;
  }
//std::cout << "No food recv count for player " << *this << "\n";
//  Assert(0);
  return 0;
}
*/

void Player::EatFood(Food* f)
{
  Ve1Object* owner = f->GetOwner();
  Assert(owner);
  // This player is NOT the local player. Only the local player can be
  //  the owner.
  Assert(owner != this);
  Assert(GetLocalPlayer() != this);

  // Inc count of this (recipient) player on server
  ChangeObjCount(GetId(), FOOD_RECEIVED_KEY, +1); // for expt
  ChangeObjCount(GetId(), SCORE_KEY, +1);

  // Inc count of food given by the local player
  ChangeObjCount(GetLocalPlayerId(), FOOD_GIVEN_KEY, +1); // for expt
  ChangeObjCount(GetLocalPlayerId(), SCORE_KEY, +1);

  std::string recipName = GetPlayerName(GetId());

  /*
  // Show msg to local player
  LurkMsg lm("You gave some food to " + recipName + "!", 
    LURK_FG, LURK_BG, AMJU_CENTRE); 
  TheLurker::Instance()->Queue(lm);    
  */

  /*
  // Send message from 'the game' to this (recipient) player
  std::string otherPlayer = GetPlayerName(owner->GetId());
  if (otherPlayer.empty())
  {
    otherPlayer = "That other player";
  }
  // TODO no good, should be only one message however many foods are given.
  TheMsgManager::Instance()->SendMsg(MsgManager::SYSTEM_SENDER, GetId(), otherPlayer + " gave you some food!");
  */

  f->SetHidden(true);
  TheSoundManager::Instance()->PlayWav("burp.wav"); // TODO
}

void Player::OnCollidePlayer(Player* otherPlayer)
{
  if (!IsLocalPlayer())
  {
    return;
  }

  std::cout << "Local player collided with player " << otherPlayer->m_name << "\n";

  if (m_carriedFood.empty())
  {
    std::cout << "Bad luck, we don't have food to give.\n";
    return;
  }

  Food* f = dynamic_cast<Food*>(m_carriedFood.back().GetPtr());
  Assert(f);
  m_carriedFood.pop_back(); // it's a deque, we add with push_front
  otherPlayer->EatFood(f);
}

void Player::OnCollideFood(Food* f)
{
  Assert(!f->IsHidden());

  // Single player mode: eat the food.
  GameMode gm = GetGameMode();
  if (gm == AMJU_MODE_SINGLE)
  {
    f->SetHidden(true);
    TheSoundManager::Instance()->PlayWav("burp.wav"); // TODO
    // Inc count of this (recipient) player on server
    ChangeObjCount(GetId(), FOOD_EATEN_KEY, +1); // for expt
    ChangeObjCount(GetId(), SCORE_KEY, +1);
  }
  else if (gm == AMJU_MODE_MULTI)
  {
    // Local player: picks up food if not currently picked up.
    Ve1Object* owner = f->GetOwner();

    if (owner == this)
    {
      // We are already carrying this food
      Assert(std::find(m_carriedFood.begin(), m_carriedFood.end(), f) != m_carriedFood.end());
      return;
    }
    else if (IsLocalPlayer())
    {
      Assert(!owner);

      // Noone is carrying the food. 
      // We pick up the food. 
      // Only show this msg the first time around
      FirstTimeMsgThisSession("You picked up some food!", UNIQUE_MSG_ID, false);

      // TODO Sound every time
      TheSoundManager::Instance()->PlayWav("pickup.wav"); // TODO

      /* Not:
      LurkMsg lm("You picked up some food!", LURK_FG, LURK_BG, AMJU_CENTRE); 
      TheLurker::Instance()->Queue(lm);    
      */
      f->SetOwner(this);
      m_carriedFood.push_front(f);
    }
  }  

  // TODO Given and received counts
  //gsm->SetFoods(GetFoodRecvCount());    
}

void Player::OnCollideBaddie(Baddie* baddie)
{
  // Only worry about the local player
  Assert(IsLocalPlayer());

  // Is it lethal? If so, die immediately

  // Lose some points

  // Dead ?
}

bool GetNameForPlayer(int objId, std::string* r)
{
  *r = GetPlayerName(objId);
  return !(r->empty());
}
}
