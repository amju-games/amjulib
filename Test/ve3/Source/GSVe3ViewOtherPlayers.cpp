#include <SoundManager.h>
#include <AmjuGL.h>
#include <Timer.h>
#include <Game.h>
#include <StringUtils.h>
#include "GSVe3ViewOtherPlayers.h"
#include "LayerGroup.h"
#include "Player.h"
#include "ObjectManager.h"
#include "GameConsts.h"
#include "LocalPlayer.h"
#include "LurkMsg.h"
#include "HeartCount.h"
#include "GSVe3Guestbook.h"
#include "MsgManager.h"
#include "GSVe3MakeTradeRequest.h"

namespace Amju
{
GSVe3ViewOtherPlayers::GSVe3ViewOtherPlayers()
{
  m_showLurk = true;
}

void GSVe3ViewOtherPlayers::Update()
{
  GSGui::Update();
  m_spriteNode.Update();
  TheLurker::Instance()->Update();
}

void GSVe3ViewOtherPlayers::Draw()
{
  GSGui::Draw();

}

void GSVe3ViewOtherPlayers::Draw2d()
{
  // Draw player
  AmjuGL::PushMatrix();
  // Scale for 'breathing' effect..?
  AmjuGL::RotateX(90.0f); 
  static float f = 0;
  f += TheTimer::Instance()->GetDt();
  float s = sin(f) * 0.001f;
  AmjuGL::Translate(-0.6f, 0, -0.4f);
  AmjuGL::Scale(0.01f, 1, 0.01f);
  AmjuGL::Scale(1, 1, 1.0f + s);

  m_spriteNode.Draw();
  AmjuGL::PopMatrix();

  // Draw over character if necessary
  GSGui::Draw2d();
}

static void OnBack()
{
  TheGSVe3ViewOtherPlayers::Instance()->GoBack();
}

static void OnPrevPlayer()
{
  TheGSVe3ViewOtherPlayers::Instance()->PrevPlayer();
}

static void OnNextPlayer()
{
  TheGSVe3ViewOtherPlayers::Instance()->NextPlayer();
}

void GSVe3ViewOtherPlayers::NextPlayer()
{
  Player* localPlayer = GetLocalPlayer();

  GameObjects* gos = TheObjectManager::Instance()->GetGameObjects();
  int id = -1;
  if (m_player)
  {
    id = m_player->GetId();
  }

  // Remember first player, go to if we reach the end
  Player* first = 0;
  bool found = false;

  for (auto it = gos->begin(); it != gos->end(); ++it)
  {
    GameObject* go = it->second;
    Player* p = dynamic_cast<Player*>(go);
    if (p == localPlayer)
    {
      continue;
    }

    if (p && !first)
    {
      first = p;
    }

    if (p && p->GetId() > id)
    {
      m_player = p;
      found = true;
      break;
    }
  }

  if (!found)
  {
    m_player = first; // wrap around if poss
  }

  if (m_player)
  {
    ShowPlayer(m_player, m_gui);
  }
}


void GSVe3ViewOtherPlayers::PrevPlayer()
{
  Player* localPlayer = GetLocalPlayer();

  GameObjects* gos = TheObjectManager::Instance()->GetGameObjects();
  int id = -1;
  if (m_player)
  {
    id = m_player->GetId();
  }

  // Remember first player, go to if we reach the end
  Player* last = 0;
  bool found = false;

  for (auto it = gos->rbegin(); it != gos->rend(); ++it)
  {
    GameObject* go = it->second;
    Player* p = dynamic_cast<Player*>(go);
    if (p == localPlayer)
    {
      continue;
    }

    if (p && !last)
    {
      last = p;
    }

    if (p && p->GetId() < id)
    {
      m_player = p;
      found = true;
      break;
    }
  }

  if (!found)
  {
    m_player = last;
  }

  if (m_player)
  {
    ShowPlayer(m_player, m_gui);
  }
}

static void OnGiveFood()
{
  TheGSVe3ViewOtherPlayers::Instance()->OnGiveFood();
}

static void OnGiveTreasure()
{
  TheGSVe3ViewOtherPlayers::Instance()->OnGiveTreasure();
}

static void OnSeeGuestbook()
{
  TheGSVe3ViewOtherPlayers::Instance()->OnSeeGuestbook();
}

void GSVe3ViewOtherPlayers::OnGiveFood()
{
  // New trading mechanic: send a message to trade food for treasure
  GSVe3MakeTradeRequest* mtr = TheGSVe3MakeTradeRequest::Instance();
  mtr->SetPrevState(this);
  mtr->SetPlayer(m_player);
  // Set up trade type
  mtr->SetTradeType(TRADE_FOOD_FOR_TREASURE);
  TheGame::Instance()->SetCurrentState(mtr);

  /*
  // Check we have some
  Player* p = GetLocalPlayer();
  Assert(p);
  if (p->Exists(FOOD_STORED_KEY))  
  {
    int food = ToInt(p->GetVal(FOOD_STORED_KEY));
    if (food < 1)
    {
      // Show lurk msg - can't give food!
      LurkMsg lm("You don't have any food to give!", LURK_FG, LURK_BG, AMJU_CENTRE); 
      TheLurker::Instance()->Queue(lm);    
    }
    else
    {
      // Decremenet local player food, add one to the other player's food count
      ChangePlayerCount(FOOD_STORED_KEY, -1);
      ChangeObjCount(m_player->GetId(), FOOD_STORED_KEY, +1);

      // Also change lifetime counters
      ChangePlayerCount(FOOD_GIVEN_KEY, +1);
      ChangeObjCount(m_player->GetId(), FOOD_RECEIVED_KEY, +1);

      ShowPlayer(m_player, m_gui); // to update GUI

      TheSoundManager::Instance()->PlayWav("sound/kiss.wav");

      std::string str = "You gave some food to " + m_player->GetName() + "!";
      LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE); 
      TheLurker::Instance()->Queue(lm);    

      // Add a message (to guestbook?) so player will know what happened
      std::string msg = p->GetName() + " gave some food to " + m_player->GetName() + "!";
      MsgManager* mm = TheMsgManager::Instance();
      mm->SendMsg(p->GetId(), m_player->GetId(), msg);
      // Also a message in our own guestbook?
      mm->SendMsg(m_player->GetId(), p->GetId(), str);
      // TODO Get this msg into guestbook - send a request for new msgs
    }
  }
  */
}

void GSVe3ViewOtherPlayers::OnGiveTreasure()
{
  // New trading mechanic: send a message to trade food for treasure
  GSVe3MakeTradeRequest* mtr = TheGSVe3MakeTradeRequest::Instance();
  mtr->SetPrevState(this);
  mtr->SetPlayer(m_player);
  // Set up trade type
  mtr->SetTradeType(TRADE_TREASURE_FOR_FOOD);
  TheGame::Instance()->SetCurrentState(mtr);

  /*
  // Check we have some
  Player* p = GetLocalPlayer();
  Assert(p);
  if (p->Exists(TREASURE_KEY))  
  {
    int tr = ToInt(p->GetVal(TREASURE_KEY));
    if (tr < 1)
    {
      LurkMsg lm("You don't have any treasure to give!", LURK_FG, LURK_BG, AMJU_CENTRE); 
      TheLurker::Instance()->Queue(lm);    
    }
    else
    {
      // Decremenet local player treasure, add one to this other player's treasure
      ChangePlayerCount(TREASURE_KEY, -1);
      ChangeObjCount(m_player->GetId(), TREASURE_KEY, +1);

      // Also change lifetime counters
      ChangePlayerCount(TREASURE_GIVEN_KEY, +1);
      ChangeObjCount(m_player->GetId(), TREASURE_RECEIVED_KEY, +1);

      ShowPlayer(m_player, m_gui); // to update GUI

      TheSoundManager::Instance()->PlayWav("sound/cashreg.wav");

      std::string str = "You gave some treasure to " + m_player->GetName() + "!";
      LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE); 
      TheLurker::Instance()->Queue(lm);    

      // Add a message (to guestbook?) so player will know what happened
      std::string msg = p->GetName() + " gave some treasure to " + m_player->GetName() + "!";
      MsgManager* mm = TheMsgManager::Instance();
      mm->SendMsg(p->GetId(), m_player->GetId(), msg);
      // Also a message in our own guestbook?
      mm->SendMsg(m_player->GetId(), p->GetId(), str);
      // TODO Get this msg into guestbook - send a request for new msgs
    }
  }
  */
}

void GSVe3ViewOtherPlayers::OnSeeGuestbook()
{
  GSVe3Guestbook* g = TheGSVe3Guestbook::Instance();
  g->SetPrevState(TheGSVe3ViewOtherPlayers::Instance());
  g->SetPlayer(m_player);
  TheGame::Instance()->SetCurrentState(g);
}

void GSVe3ViewOtherPlayers::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-viewotherplayers.txt");
  Assert(m_gui);

  GetElementByName(m_gui, "back-button")->SetCommand(OnBack);
  GetElementByName(m_gui, "prev-player-button")->SetCommand(OnPrevPlayer);
  GetElementByName(m_gui, "next-player-button")->SetCommand(OnNextPlayer);

  GetElementByName(m_gui, "give-food-button")->SetCommand(Amju::OnGiveFood);
  GetElementByName(m_gui, "give-treasure-button")->SetCommand(Amju::OnGiveTreasure);
  GetElementByName(m_gui, "see-guestbook-button")->SetCommand(Amju::OnSeeGuestbook);

  // Initialise character
  if (m_player)
  {
    ShowPlayer(m_player, m_gui);
  }
  else
  {
    NextPlayer();
  }
}

} // namespace
