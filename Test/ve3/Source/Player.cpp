#include <AmjuFirst.h>
#include <Game.h>
#include <AmjuGL.h>
#include <SoundManager.h>
#include <iostream>
#include <File.h>
#include <StringUtils.h>
#include <DegRad.h>
#include <Timer.h>
#include <GuiText.h>
#include <DrawAABB.h>
#include <AmjuRand.h>
#include <GameObjectFactory.h>
#include <Shadow.h>
#include <CollisionMesh.h>
#include "Player.h"
#include "Ve1Character.h"
#include "Ve1SceneGraph.h"
#include "Useful.h"
#include "Terrain.h"
#include "LocalPlayer.h"
#include "GSMain.h"
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
#include "PlayerNames.h"
#include "Ve1SpriteNode.h"
#include "Food.h"
#include "GS2dAvatarMod.h"
#include "MsgManager.h"
#include "GameMode.h"
#include "GameConsts.h"
#include "Baddie.h"
#include "GSDeath.h"
#include "Bullet.h"
#include "UnCollide.h"
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
    m_heartScale = 1.0f;

    SetBlended(true);
    m_text.SetTextSize(5.0f); // TODO CONFIG
    static const float MAX_NAME_WIDTH = 4.0f; // minimise this to reduce overdraw - calc from text
    m_text.SetSize(Vec2f(MAX_NAME_WIDTH, 1.0f));
    m_text.SetJust(GuiText::AMJU_JUST_LEFT);
    m_text.SetFgCol(Colour(1, 1, 1, 1));
    //m_text.SetInverse(true);
    //m_text.SetDrawBg(true);
  }

  virtual void Update()
  {
    const float HEARTBEAT = 3.0f;
    m_heartScale += TheTimer::Instance()->GetDt() * HEARTBEAT;
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
 
      int health = m_player->GetHealth(); 
      std::string name = ToString(health) + "  " + m_player->GetName();
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

      // World is drawn on x-y plane
      m.RotateX((float)(-M_PI / 2.0));

      Vec3f tr(m_combined[12], m_combined[13], m_combined[14]);
      m.TranslateKeepRotation(tr);
      AmjuGL::MultMatrix(m);
      static const float SCALE_FACTOR = 20.0f;
      //static const float MAX_NAME_WIDTH = 4.0f; // minimise this to reduce overdraw - calc from text
      // World is big compared to -1..1 coords
      AmjuGL::Scale(SCALE_FACTOR, SCALE_FACTOR, 1);  
      //float x = MAX_NAME_WIDTH * -0.5f;
      AmjuGL::Translate(0, 3.0f, 0); // TODO CONFIG

      static GuiImage* heartImg = 0;
      static const float HEART_SIZE = 1.2f;
      if (!heartImg)
      {
        heartImg = new GuiImage;
        heartImg->SetTexture((Texture*)TheResourceManager::Instance()->GetRes("heart16-red.png"));
        heartImg->SetLocalPos(Vec2f(-HEART_SIZE, 0));
      }
      const float HEART_SIZE_VARY = 0.1f;
      float s = sin(m_heartScale) * HEART_SIZE_VARY + 1.0f;
      float h = HEART_SIZE * s;
      heartImg->SetSize(Vec2f(h, h));
/*
      PushColour();
      if (health < 2)
      {
        MultColour(Colour(0, 0, 1, 1));
      }
*/
      heartImg->Draw();
/*
      PopColour();
*/
      m_text.Draw();

      AmjuGL::PopMatrix();
      AmjuGL::PopAttrib();
    }
  }
 
protected:
  RCPtr<Player> m_player;
  GuiText m_text;
  float m_heartScale;
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

//  m_lastFoodCount = -1;
  m_isDead = false;
//  m_totalFoods = -1; // not set yet

  Ve1SpriteNode* sn = new Ve1SpriteNode;
  sn->SetObj(this);
  SetSceneNode(sn);

  m_maxHealth = 3; // Default - TODO
  m_health = m_maxHealth;
  m_hitTimer = 0;
  m_flashTimer = 0;

  // Whoa no!!!
  ///ResetHealth();

  // Load particle effect
  File fight;
  if (!fight.OpenRead("fighteffect.txt"))
  {
    fight.ReportError("Failed to load effect");
    Assert(0);
  }
  m_effect = new AttackEffect;
  if (!m_effect->Load(&fight))
  {
    fight.ReportError("Failed to load effect");
    Assert(0);
  }
  m_effect->SetVisible(true);
}

int Player::GetHealth() const
{
  return m_health;
}

int Player::GetMaxHealth() const
{
  return m_maxHealth;
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

//void Player::SetDir(float degs)
//{
//  Ve1ObjectChar::SetDir(degs);
//
//  // Makes AABBs inaccurate for long objects etc
//#ifdef ALLOW_ARBITRARY_ANGLES
//  if (m_carrying)
//  {
//    m_carrying->SetDir(degs);
//  }
//#endif
//}

bool Player::IsLoggedIn() const
{
  return m_isLoggedIn;
}

bool Player::Load(File* f)
{
  m_maxHealth = 3; // Default until we get a new value from server
  m_health = m_maxHealth;

  // Load particle effect
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



  // Create Scene Node, but don't attach to SceneGraph until needed  

  // TODO We can't until we call SetFromCharacterName in SetKeyVal

  //PlayerSceneNode* psn = new PlayerSceneNode(this);
  //m_sceneNode = psn;

  //if (!m_shadow->Load(f))
  //{
  //  return false;
  //}

  // Load arrow scene node
/*
  ObjMesh* arrowMesh = (ObjMesh*)TheResourceManager::Instance()->GetRes("arrow.obj"); 
  Assert(arrowMesh);
  m_arrow = new SceneMesh;
  m_arrow->SetMesh(arrowMesh);
*/

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
  // Can't carry food across room boundary..??
  m_carriedFood.clear();

  Ve1ObjectChar::OnLocationEntry();

/*
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  if (root)
  {
    root->AddChild(m_arrow.GetPtr());
    SetArrowVis(false);
  }
*/

  if (m_sceneNode && !m_nameTag) // so only does it first time
  {
    m_nameTag = new PlayerNameNode(this);
    m_sceneNode->AddChild(m_nameTag.GetPtr());
  
    Assert(m_effect);
    m_sceneNode->AddChild(m_effect.GetPtr());
    m_effect->SetVisible(false);
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

  if (key == SCORE_KEY)
  {
    if (IsLocalPlayer())
    {
      int score = ToInt(GetVal(SCORE_KEY));
      gsm->SetScore(score); 
    }
  }
  else if (key == HEALTH_KEY)
  {
    // Set health if we are not the local player. This is so we can
    //  see the health of other players
    //???if (!IsLocalPlayer())
    {
      m_health = ToInt(val);
    }
  }
  else if (key == "loggedin")
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
  else if (key == LAST_MSG_SENT_KEY)
  {
    if (IsLocalPlayer())
    {
      int id = ToInt(val);
      static int lastId = 0;
      if (id > lastId)
      {
        TheMsgManager::Instance()->CheckForNewMsgs();
        lastId = id;
      }
    }
  }
  else if (key == TREASURE_KEY)
  {
    if (IsLocalPlayer())
    {
      gsm->SetTreasure(ToInt(val));
    }
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
  float dt = TheTimer::Instance()->GetDt();

  if (m_isLoggedIn)
  {
    Ve1ObjectChar::Update();

    if (m_hitTimer > 0)
    {
      Matrix m;
      const float JIGGLE = 2.0f;
      Vec3f jiggle(Rnd(-JIGGLE, JIGGLE), 0, Rnd(-JIGGLE, JIGGLE));
      m.Translate(jiggle);
      m_sceneNode->MultLocalTransform(m);
    }
  }
  else if (m_sceneNode)
  {
    Matrix m;
    m.Translate(m_pos);
    m_sceneNode->SetLocalTransform(m);

    // Set shadow AABB to same as Scene Node so we don't cull it by mistake
    //m_shadow->SetAABB(*(m_sceneNode->GetAABB()));

    static const float XSIZE = ROConfig()->GetFloat("player-aabb-x", 40.0f);
    static const float YSIZE = ROConfig()->GetFloat("player-aabb-y", 10.0f);
    static const float ZSIZE = ROConfig()->GetFloat("player-aabb-z", 40.0f);

    const Vec2f& scale = ((Ve1SpriteNode*)m_sceneNode.GetPtr())->GetScale();
    float xsize = XSIZE * scale.x;
    float zsize = ZSIZE * scale.y;

    m_aabb.Set(
      m_pos.x - xsize, m_pos.x + xsize,
      m_pos.y, m_pos.y + YSIZE,
      m_pos.z - zsize, m_pos.z + zsize);

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
    // Set name tag AABB to same as Scene Node so we don't cull it by mistake
    m_nameTag->SetAABB(*(m_sceneNode->GetAABB()));

    // Set colour depending on health - only if logged in
    //if (IsLoggedIn())// ???
    //{
      Colour col(1, 1, 1, 1);
      if (m_hitTimer > 0)
      {
        m_hitTimer -= dt;
        col = m_damageColour; 
        if (m_hitTimer <= 0)
        {
          m_hitTimer = 0;

          // Dead ?
          if (m_health <= 0)
          {
            TheGame::Instance()->SetCurrentState(TheGSDeath::Instance());
          }
        }
      }
      else if (m_health == 1) // TODO TEMP TEST
      {
        // Flash if health low
        m_flashTimer -= dt;
        static const float FLASH = 0.6f; // TODO CONFIG
        if (m_flashTimer <= 0)
        {
          m_flashTimer = FLASH; 
        }
        if (m_flashTimer > FLASH * 0.5f)
        {
          col = Colour(1, 0, 0, 1); // flash red? green also looks good - maybe for magic attack etc...
        }
      }
      m_sceneNode->SetColour(col);
    //}
  }

  if (m_ignorePortalId != -1)
  {
    GameObject* g = TheGame::Instance()->GetGameObject(m_ignorePortalId);
    if (g)
    {
      const AABB& aabb = g->GetAABB();
      if (!GetAABB().Intersects(aabb))
      {
        // No longer intersecting portal
        m_ignorePortalId = -1;
      }
    }
    else
    {
      m_ignorePortalId = -1; // ?
    }
  }

  //static GSMain* gsm = TheGSMain::Instance();
  //if (IsLocalPlayer())
  //{
  //  GameMode gm = GetGameMode();
  //  if (gm == AMJU_MODE_SINGLE)
  //  {
  //    // Score is food eaten
  //    gsm->SetScore(m_stamina); // TODO
  //  }
  //  else if (gm == AMJU_MODE_MULTI)
  //  {
  //    // Score is food given + food recv ??
  //    gsm->SetScore(m_stamina); // TODO
  //  }

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
//  }
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
    // Hmm, makes it difficult to shoot baddies 
    //AddMenuItem("Change look", new CommandAvatarMod);
  }
  else if (IsLoggedIn())
  {
  }
  else
  {
    // TODO put back leaving messages
    //menu->AddChild(new GuiMenuItem("Leave a message for " + GetName(), new CommandTalk(this)));
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

/*
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

  // Inc health of recipient player
  ChangeObjCount(GetId(), HEALTH_KEY, +1);

  // Inc count of food given by the local player
  ChangeObjCount(GetLocalPlayerId(), FOOD_GIVEN_KEY, +1); // for expt
  ChangeObjCount(GetLocalPlayerId(), SCORE_KEY, +1);

  std::string recipName = GetPlayerName(GetId());

  std::string otherPlayer = GetPlayerName(owner->GetId());
  if (otherPlayer.empty())
  {
    otherPlayer = "Someone";
  }
  std::string str = otherPlayer + " gave some food to " + recipName + "!";
  TheMsgManager::Instance()->SendMsg(MsgManager::SYSTEM_SENDER, MsgManager::BROADCAST_RECIP, str);

  f->SetHidden(true);
  TheSoundManager::Instance()->PlayWav("sound/burp.wav"); // TODO
}
*/

void Player::OnCollidePlayer(Player* otherPlayer)
{
  // Ve3: not expecting this to happen!
  Assert(0);

  if (!IsLocalPlayer() && otherPlayer->IsLocalPlayer())
  {
    otherPlayer->OnCollidePlayer(this);
    return;
  }

  std::cout << "Local player collided with player " << otherPlayer->m_name << "\n";

/*
  if (m_carriedFood.empty())
  {
    std::cout << "Bad luck, we don't have food to give.\n";
    return;
  }

  Food* f = dynamic_cast<Food*>(m_carriedFood.back().GetPtr());
  Assert(f);
  m_carriedFood.pop_back(); // it's a deque, we add with push_front
  otherPlayer->EatFood(f);
*/
}

void OnEatNo()
{
  LurkMsg lm("OK, you can eat it later, or give it to someone else if you like!", LURK_FG, LURK_BG, AMJU_CENTRE); 
  TheLurker::Instance()->Queue(lm);    
  // Inc player food count
  ChangePlayerCount(FOOD_STORED_KEY, +1);
}

void OnEatYes()
{
  // TODO Different health points for different food types?
  LurkMsg lm("Yum yum! You got +1 health point!", LURK_FG, LURK_BG, AMJU_CENTRE); 
  TheLurker::Instance()->Queue(lm);    

  // TODO This must update the server but immediately update on the client
  ChangePlayerCount(HEALTH_KEY, +1);
}

void Player::OnCollideFood(Food* f)
{
  Assert(!f->IsHidden());

  // New for VE3:
  f->SetHidden(true);
  // You can give this to someone, keep it, or eat it now (increasing health)
  TheLurker::Instance()->ShowYesNo("You picked up some food! Would you like to eat it now?", LURK_FG, LURK_BG, OnEatNo, OnEatYes);

/*
  // Single player mode: eat the food.
  GameMode gm = GetGameMode();
  if (gm == AMJU_MODE_SINGLE)
  {
    f->SetHidden(true);
    TheSoundManager::Instance()->PlayWav("sound/burp.wav"); // TODO
    // Inc count of this (recipient) player on server
    ChangeObjCount(GetId(), FOOD_STORED_KEY, +1); // for expt
    ChangeObjCount(GetId(), SCORE_KEY, +1);

    // Get some health
    m_health++; // single player, no need to upload health, right??
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
      // Annoying
      //FirstTimeMsgThisSession("You picked up some food!", UNIQUE_MSG_ID, false);

      // TODO Sound every time
      TheSoundManager::Instance()->PlayWav("sound/pickup.wav"); // TODO

      // Not:
      //LurkMsg lm("You picked up some food!", LURK_FG, LURK_BG, AMJU_CENTRE); 
      //TheLurker::Instance()->Queue(lm);    
      f->SetOwner(this);
      m_carriedFood.push_front(f);
    }
  }  
*/

  // TODO Given and received counts
  //gsm->SetFoods(GetFoodRecvCount());    
}

void Player::OnCollideBaddie(Baddie* baddie)
{
  // Only worry about the local player
  Assert(IsLocalPlayer());

  if (m_hitTimer > 0)
  {
    // Already hit
    return;
  }

  // Is it lethal? If so, die immediately

  // Lose some points
  int damage = baddie->GetDamage();
  if (damage != 0) // negative damage?!?!?!?!?!
  {
    if (damage < 0)
    {
      m_effect->SetColour(Colour(0, 1, 0, 1));
      m_damageColour = Colour(0, 1, 0, 1); 
    }
    else
    {
      m_effect->SetColour(Colour(1, 0, 0, 1));
      m_damageColour = Colour(1, 0, 0, 1); 
    }

    ShowAttacked();
    TheSoundManager::Instance()->PlayWav("sound/gasp.wav"); 

    m_health -= damage;

    // Send this to server so we can see other players' health
    TheObjectUpdater::Instance()->SendUpdateReq(
      GetId(), SET_KEY(HEALTH_KEY), ToString(m_health));

    // Could also send a system message
    std::string str;
    if (GetNameForPlayer(GetId(), &str)) // TODO IS this the best way to do this?
    {
      std::string attackStr = baddie->GetAttackString();
      if (m_health > 0)
      {
        // Add health and room info for other players to help
        str += " " + attackStr + " Health: " + ToString(m_health) + " Room: " +
          ToString(GetLocalPlayerLocation());
      }
      else if (m_health < 1)
      {
        str += " died!"; // TODO variety of msgs
      }
      
      TheMsgManager::Instance()->SendMsg(
        MsgManager::SYSTEM_SENDER, MsgManager::BROADCAST_RECIP, str);
    }

    static const float MAX_HIT_TIME = 2.0f; // TODO CONFIG
    m_hitTimer = MAX_HIT_TIME;  
    // Check for death in update, after we have shown the effect of the hit.

    // TODO This doesn't work anyway
    // Can't go through - it may be acting as an obstacle
    //UnCollide(this, GetOldPos(), (*baddie->GetAABB()));
  }

  // Slow down
  Vec3f vel = GetVel();
  vel *= 0.5f; // TODO Config
  SetVel(vel); 
}

void Player::ShootBaddie(Baddie* baddie)
{
  Vec3f pos = baddie->GetPos();
  // Add a bit of random variation
  pos.x += Rnd(-20.0f, 20.0f);
  pos.z += Rnd(-20.0f, 20.0f);

  // TODO Would also like the bullet path to be a bit curved

  Vec3f vel = pos - m_pos;
  vel.Normalise();
  static const float BULLET_SPEED = 400.0f; // TODO CONFIG
  vel *= BULLET_SPEED; 
  Bullet* b = new Bullet;
  b->SetVel(vel);
  b->SetPos(m_pos);

  TheGame::Instance()->AddGameObject(b);
}

void Player::ResetHealth()
{
  if (m_health < m_maxHealth)
  {
    m_health = m_maxHealth;
    TheObjectUpdater::Instance()->SendUpdateReq(GetId(), SET_KEY(HEALTH_KEY), ToString(m_health));
  }
}

bool GetNameForPlayer(int objId, std::string* r)
{
  *r = GetPlayerName(objId);
  return !(r->empty());
}
}

