#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <AmjuTime.h>
#include <TimePeriod.h>
#include "GSToday.h"
#include "GameMode.h"
#include "GSCogTestMenu.h"
#include "GSTitle.h"
#include "GSTodaysTask.h"
#include "GSLogout.h"
#include "GSAchievements.h"
#include "GSCogResults.h"
#include "GSStartGame.h"
#include "GS2dAvatarMod.h"
#include "GSWaitForNewLocation.h"
#include "CogTestNag.h"
#include "LocalPlayer.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnDoTests()
{
  GSCogTestMenu* gtm = TheGSCogTestMenu::Instance();
  gtm->SetPrevState(TheGSToday::Instance());
  TheGame::Instance()->SetCurrentState(gtm);
}

//static void OnTodayViewAchievements()
//{
//  TheGSAchievements::Instance()->SetPrevState(TheGSToday::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSAchievements::Instance());
//}

//static void OnTodayModAvatar()
//{
//  static GS2dAvatarMod* am = TheGS2dAvatarMod::Instance();
//  static GSWaitForNewLocation* wnl = TheGSWaitForNewLocation::Instance();
//  static Game* game = TheGame::Instance();
//
//  if (GetLocalPlayer())
//  {
//    am->SetPrevState(TheGSToday::Instance());
//    game->SetCurrentState(am);
//  }
//  else
//  {
//    // Wait for player to download
//    wnl->SetPrevState(TheGSToday::Instance());
//    game->SetCurrentState(wnl);
//  }
//}

static void OnTodayViewResults()
{
  TheGSCogResults::Instance()->SetPrevState(TheGSToday::Instance());
  TheGame::Instance()->SetCurrentState(TheGSCogResults::Instance());
}

static void OnTodayPlaySingle()
{
  SetGameMode(AMJU_MODE_SINGLE);
  TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());
}

static void OnTodayPlayMulti()
{
  SetGameMode(AMJU_MODE_MULTI);
  TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());
}

static void OnTodayOk()
{
  GameState* state = 0;

  GameMode gm = GetGameMode();
  if (gm == AMJU_MODE_NO_GAME)
  {
    // If no game day and it's not a research session, there is nothing to do.
    // If no game day, go straight to cog test.
    if (DoCogTests())
    {
      state = TheGSCogTestMenu::Instance();
    }
    else
    {
      // Explain state ?? No, the text in this state explains.
      TheGSLogout::Instance()->SetPrevState(TheGSTitle::Instance());
      state = TheGSLogout::Instance();
    }
  }
  else if (gm == AMJU_MODE_MULTI || gm == AMJU_MODE_SINGLE)
  {
    // If it's a game day, start playing
    // (skipping TheGSTodaysTask, which seems like an unnecessary extra page)
    state = TheGSStartGame::Instance();
  } 
  else
  {
std::cout << "Unexpected game mode!!\n";
    Assert(0);
  }
  TheGame::Instance()->SetCurrentState(state);
}

GSToday::GSToday()
{
}

void GSToday::Update()
{
  GSGui::Update();

}

void GSToday::Draw()
{
  GSGui::Draw();

}

void GSToday::Draw2d()
{
  GSGui::Draw2d();
}

void GSToday::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-today.txt");
  Assert(m_gui);

  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetCommand(OnTodayOk);
  GameMode gm = GetGameMode();
  if (gm == AMJU_MODE_MULTI || gm == AMJU_MODE_SINGLE)
  {
    ok->SetText("play!");
  }

/*
  // View game stats
  GuiButton* viewAch = (GuiButton*)GetElementByName(m_gui, "view-ach-button");
  viewAch->SetCommand(OnTodayViewAchievements);
*/

  GuiButton* avatar = (GuiButton*)GetElementByName(m_gui, "mod-avatar-button");
  //avatar->SetCommand(OnTodayModAvatar);
  // Disabled for now: tricky, as local player object has to exist, so we have to wait.
  avatar->SetVisible(false);

  GuiButton* viewResults = (GuiButton*)GetElementByName(m_gui, "view-results-button");
  viewResults->SetCommand(OnTodayViewResults);

  // Do cog tests today?
  Time today = Time::Now();
  today.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));

  int numCompleted = TheCogTestResults::Instance()->
    GetNumCompletedTestsForDate(today);
  int numTodo = GetNumCogTests();
  bool cogTestsAllDoneToday = (numCompleted == numTodo);

  // Always allow tests?? 
  ////if (DoCogTests() && !cogTestsAllDoneToday)
  GuiButton* doTests = (GuiButton*)GetElementByName(m_gui, "do-tests-button");
  doTests->SetCommand(OnDoTests);
  if (cogTestsAllDoneToday)
  {
    doTests->SetIsEnabled(false); // done for today
  }

  // For admin/dev, button to play game even on non-game days
  GuiButton* playSingle = (GuiButton*)GetElementByName(m_gui, "play-single-button");
  GuiButton* playMulti = (GuiButton*)GetElementByName(m_gui, "play-multi-button");
  playSingle->SetCommand(OnTodayPlaySingle);
  playMulti->SetCommand(OnTodayPlayMulti);

#ifndef _DEBUG
  playSingle->SetVisible(false);
  playMulti->SetVisible(false);
#endif

  GuiText* dateText = (GuiText*)GetElementByName(m_gui, "date-text");
  dateText->SetText(today.ToStringJustDate()); 

  GuiText* text = (GuiText*)GetElementByName(m_gui, "long-text");
  std::string str;

  // The cases are:
  // No game mode, and we must do test
  // No game mode, and no need to do test
  // Multi-player mode. May need to do test, will be prompted in game.
  if (gm == AMJU_MODE_NO_GAME)
  {
    // If no game day and test required, go straight to cog test.
    if (cogTestsAllDoneToday)
    {
      str = "You have done the tests, and there is nothing else for you to do today. Thank you!";
    }
    else if (DoCogTests())
    {
      str = "Today, please take a few tests. This should take about ten minutes. Thank you!";
      // TODO Get next date from the ResearchCalendar -- show in final thanks page
    }
    else
    {
      // If no game day and it's not a research session, there is nothing to do.
      str = "There is nothing for you to do today, but you can take the tests if you would like."
        " Thank you for participating!";
      // TODO Get next date from the ResearchCalendar
    }
  }
  else if (gm == AMJU_MODE_MULTI)
  {
    // If it's a game day, go to main state.
    str = "Today, please play the game, (multi-player mode).";
    if (cogTestsAllDoneToday)
    {
      str += " Thanks for doing the tests today!";
    }
    else if (DoCogTests())
    {
      str += "\n\nYou will be asked to take a few tests during the game. Thanks for your help!";
    }
    else
    {
      str += "\n\nThere are no tests for you today!";
    }
  } 
  else if (gm == AMJU_MODE_SINGLE)
  {
    // If it's a game day, go to main state.
    str = "Please play the game. Today it is a single-player game.";
    if (cogTestsAllDoneToday)
    {
      str += " Thanks for doing the tests today!";
    }
    else if (DoCogTests())
    {
      str += "\n\nYou will be asked to take a few tests during the game. Thanks for your help!";
    }
    else
    {
      str += "\n\nThere are no tests for you today!";
    }
  }
  else
  {
std::cout << "Unexpected game mode!!\n";
    Assert(0);
  }
  text->SetText(str);
}

} // namespace
