#include <AmjuGL.h>
#include <Game.h>
#include <Timer.h>
#include <TimePeriod.h>
#include "GSVe3MyStats.h"
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
#include "GSCalendar.h"
#include "GSCogResults.h"
#include "GSCogTestMenu.h"
#include "CogTestResults.h"
#include "CogTestNag.h"

namespace Amju
{
static void OnTestResults(GuiElement*)
{
  GSCogResults* cr = TheGSCogResults::Instance();
  cr->SetPrevState(TheGSVe3MyStats::Instance());
  TheGame::Instance()->SetCurrentState(cr);
}

static void OnDoTests(GuiElement*)
{
  // When done, come back to home page
  // This is broken
  TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
}

static void OnCalendar(GuiElement*)
{
  GSCalendar* cal = TheGSCalendar::Instance();
  cal->SetPrevState(TheGSVe3MyStats::Instance());
  TheGame::Instance()->SetCurrentState(cal);
}

GSVe3MyStats::GSVe3MyStats()
{
  m_showLurk = true;
}

void GSVe3MyStats::RefreshGui()
{
//  Player* p = GetLocalPlayer();
//  Assert(p);
//  ShowPlayer(p, m_gui);
}

void GSVe3MyStats::Update()
{
  GSGui::Update();

  m_spriteNode.Update();
  TheLurker::Instance()->Update();

}

void GSVe3MyStats::Draw()
{
  GSGui::Draw();
}

void GSVe3MyStats::Draw2d()
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

void GSVe3MyStats::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-mystats.txt");
  Assert(m_gui);

  GetElementByName(m_gui, "calendar-button")->SetCommand(OnCalendar);
  GetElementByName(m_gui, "test-results-button")->SetCommand(OnTestResults);

  // Disable Do Tests button if done for the day
  Time today = Time::Now();
  today.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));
  bool cogTestsAllDoneToday = TheCogTestResults::Instance()->
    GetNumCompletedTestsForDate(today) == GetNumCogTests();

  GuiButton* doTests = (GuiButton*)GetElementByName(m_gui, "do-tests-button");
  doTests->SetCommand(OnDoTests);
  if (cogTestsAllDoneToday)
  {
    doTests->SetIsEnabled(false); // done for today
  }

  Player* p = GetLocalPlayer();
  Assert(p);

  //ShowPlayer(p, m_gui);
  Player* player = GetLocalPlayer();
  Assert(player);
  LayerGroups layerGroups; // store settings for each layer
  layerGroups.SetFromSprite(player->GetSprite());
  layerGroups.SetSprite(&m_spriteNode.GetSprite());
  //m_spriteNode.Update();

  SetHomeButton();
}

} // namespace
