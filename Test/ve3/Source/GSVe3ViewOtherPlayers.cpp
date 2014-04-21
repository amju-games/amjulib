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
#include "GSVe3ChooseTradeType.h"

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

static void OnMakeATrade()
{
  TheGSVe3ViewOtherPlayers::Instance()->OnMakeATrade();
}

static void OnSeeGuestbook()
{
  TheGSVe3ViewOtherPlayers::Instance()->OnSeeGuestbook();
}

void GSVe3ViewOtherPlayers::OnMakeATrade()
{
  GSVe3ChooseTradeType* ctt = TheGSVe3ChooseTradeType::Instance();
  ctt->SetPrevState(this); // cancel -> go back here
  ctt->SetOtherPlayer(m_player);
  TheGame::Instance()->SetCurrentState(ctt);
}

void GSVe3ViewOtherPlayers::OnSeeGuestbook()
{
  GSVe3Guestbook* g = TheGSVe3Guestbook::Instance();
  g->SetPrevState(TheGSVe3ViewOtherPlayers::Instance());
  g->SetPlayer(m_player);
  g->SetIsGuestbookOnly(true);
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

  GetElementByName(m_gui, "make-trade-button")->SetCommand(Amju::OnMakeATrade);
  GetElementByName(m_gui, "see-guestbook-button")->SetCommand(Amju::OnSeeGuestbook);

  // Initialise character
  if (m_player && 
      m_player->GetId() != GetLocalPlayerId()) // could happen if you log out, then in as this player
  {
    ShowPlayer(m_player, m_gui);
  }
  else
  {
    NextPlayer();
  }
}

} // namespace
