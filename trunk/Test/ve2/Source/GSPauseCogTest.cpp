#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Game.h>
#include "GSPauseCogTest.h"
#include "GSMain.h"
#include "GameConsts.h"
#include "GameMode.h"
#include "GSLogout.h"
#include "GSTitle.h"
#include "GSCogTestHelp.h"
#include "GSCogTestMenu.h"
#include "LurkMsg.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnNoDoNothing() {}

static void OnResumeButton()
{
  // TODO This may well restart the test, if OnActive resets the test.
  TheGSPauseCogTest::Instance()->GoBack();
}

static void OnQuitYes()
{
  GameMode gm = GetGameMode();
  if (gm == AMJU_MODE_NO_GAME || gm == AMJU_MODE_NONE)
  {
    // Log out, you can't go to the game. Or go to GSThanks/GSToday?
    TheGSLogout::Instance()->SetPrevState(TheGSTitle::Instance());
    TheGame::Instance()->SetCurrentState(TheGSLogout::Instance());
  }
  else
  {
    // Go back to Main, will nag again later.
    TheGame::Instance()->SetCurrentState(TheGSMain::Instance());

    LurkMsg lm("OK, but please finish taking the tests soon! I will ask you again later.", LURK_FG, LURK_BG, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);
  }
}

static void OnQuitTests()
{
  TheLurker::Instance()->ShowYesNo("Are you sure you want to quit doing the tests?", LURK_FG, LURK_BG, OnNoDoNothing, OnQuitYes);
}

static void OnSkipYes()
{
  GSCogTestMenu* ctm = TheGSCogTestMenu::Instance();
  ctm->AdvanceToNextTest();

  TheGame::Instance()->SetCurrentState(ctm); 
}

static void OnSkipThisTest()
{
  TheLurker::Instance()->ShowYesNo("Are you sure you want to skip this test?", LURK_FG, LURK_BG, OnNoDoNothing, OnSkipYes);
}

static void OnHelp()
{
  TheGame::Instance()->SetCurrentState(TheGSCogTestHelp::Instance());
}

static void OnRestartYes()
{
  // TODO This *shouldn't* restart the test - TODO fix this
  TheGSPauseCogTest::Instance()->GoBack();
}

static void OnRestartThisTest()
{
  TheLurker::Instance()->ShowYesNo("Are you sure you want to restart this test?", LURK_FG, LURK_BG, OnNoDoNothing, OnRestartYes);
}

GSPauseCogTest::GSPauseCogTest()
{
  TheLurker::Instance()->Clear();
  m_showLurk = true;
}

void GSPauseCogTest::Update()
{
  GSGui::Update();

}

void GSPauseCogTest::Draw()
{
  GSGui::Draw();

}

void GSPauseCogTest::Draw2d()
{
  GSGui::Draw2d();
}

void GSPauseCogTest::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-pause-cogtest.txt");
  Assert(m_gui);

  m_gui->GetElementByName("resume-button")->SetCommand(Amju::OnResumeButton);
  m_gui->GetElementByName("help-button")->SetCommand(Amju::OnHelp);
  m_gui->GetElementByName("restart-button")->SetCommand(Amju::OnRestartThisTest);
  m_gui->GetElementByName("skip-button")->SetCommand(Amju::OnSkipThisTest);
  m_gui->GetElementByName("quit-button")->SetCommand(Amju::OnQuitTests);
}
} // namespace
