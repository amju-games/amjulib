#include <AmjuGL.h>
#include <Game.h>
#include <Timer.h>
#include "GSVe3HomePage.h"
#include "GSVe3ViewOtherPlayers.h"
#include "GSMain.h"
#include "GS2dAvatarMod.h"
#include "GSQuitGame.h"
#include "GSVe3Guestbook.h"
#include "LocalPlayer.h"
#include "LayerGroup.h"
#include "ObjectManager.h"
#include "ObjectUpdater.h"
#include "GSWaitForNewLocation.h"
#include "LurkMsg.h"
#include "GameConsts.h"
#include "CheckForNewMessages.h"

namespace Amju
{
void OnEatButton(); // in GSMain

static void OnHomepageEatButton()
{
  OnEatButton();
  TheGSVe3HomePage::Instance()->RefreshGui();
}

static void OnSeeMyGuestbook()
{
  GSVe3Guestbook* g = TheGSVe3Guestbook::Instance();
  g->SetPrevState(TheGSVe3HomePage::Instance());
  g->SetPlayer(GetLocalPlayer());
  g->SetIsGuestbookOnly(true);
  TheGame::Instance()->SetCurrentState(g);
}

static void OnSeeMyMessages()
{
  GSVe3Guestbook* g = TheGSVe3Guestbook::Instance();
  g->SetPrevState(TheGSVe3HomePage::Instance());
  g->SetPlayer(GetLocalPlayer());
  g->SetIsGuestbookOnly(false);
  TheGame::Instance()->SetCurrentState(g);
}

static void OnQuit()
{
  TheGSQuitGame::Instance()->SetPrevState(TheGSVe3HomePage::Instance());
  TheGame::Instance()->SetCurrentState(TheGSQuitGame::Instance());
}

GSVe3HomePage::GSVe3HomePage()
{
  m_showLurk = true;
}

void GSVe3HomePage::RefreshGui()
{
  Player* p = GetLocalPlayer();
  Assert(p);
  ShowPlayer(p, m_gui);
}

void GSVe3HomePage::Update()
{
  GSGui::Update();

  m_spriteNode.Update();
  TheLurker::Instance()->Update();

  // Display Lurk Msg if we have any new msgs
  // This is not sending DB reqs to server, just checking the local queue.
  CheckForNewMessages(); 
}

void GSVe3HomePage::Draw()
{
  GSGui::Draw();
}

void GSVe3HomePage::Draw2d()
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

static void OnChangeName()
{
}

static void OnChangeLook()
{
  GS2dAvatarMod* gs = TheGS2dAvatarMod::Instance();
  gs->SetPrevState(TheGSVe3HomePage::Instance());
  TheGame::Instance()->SetCurrentState(gs);
}

static void OnExplore()
{
  GSMain* gs = TheGSMain::Instance();
  gs->SetPrevState(TheGSVe3HomePage::Instance());

  // Get location for local player
  Player* p = GetLocalPlayer();
  Assert(p);

  // We can only go exploring if we have non-zero health
  if (p->Exists(HEALTH_KEY))
  {
    int health = ToInt(p->GetVal(HEALTH_KEY));
    if (health <= 0)
    {
      // TODO Wav

      // Message - tell player how to get health
      std::string str = "Oh dear, I am sorry! You are not healthy enough to go exploring. Eat some food to get healthy!";
      LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE); 
      TheLurker::Instance()->Queue(lm);    

      return;
    }
  }

  int loc = p->GetLocation();
  SetLocalPlayerLocation(loc);

  TheGame::Instance()->SetCurrentState(TheGSWaitForNewLocation::Instance());
}

static void OnOtherPlayers()
{
  GSVe3ViewOtherPlayers* gs = TheGSVe3ViewOtherPlayers::Instance();
  gs->SetPrevState(TheGSVe3HomePage::Instance());
  TheGame::Instance()->SetCurrentState(gs);
}

void GSVe3HomePage::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-homepage.txt");
  Assert(m_gui);

  GetElementByName(m_gui, "quit-button")->SetCommand(OnQuit);
  GetElementByName(m_gui, "change-name-button")->SetCommand(OnChangeName);
  GetElementByName(m_gui, "change-look-button")->SetCommand(OnChangeLook);
  GetElementByName(m_gui, "explore-button")->SetCommand(OnExplore);
  GetElementByName(m_gui, "other-players-button")->SetCommand(OnOtherPlayers);
  GetElementByName(m_gui, "eat-button")->SetCommand(OnHomepageEatButton);
  GetElementByName(m_gui, "see-guestbook-button")->SetCommand(OnSeeMyGuestbook);
  GetElementByName(m_gui, "see-msgs-button")->SetCommand(OnSeeMyMessages);

  Player* p = GetLocalPlayer();
  Assert(p);

  ShowPlayer(p, m_gui);

  // Send req for new msgs
  TheMsgManager::Instance()->CheckForNewMsgs();
  CheckForNewMessages(); // display Lurk Msg if we have any new msgs
}

} // namespace
