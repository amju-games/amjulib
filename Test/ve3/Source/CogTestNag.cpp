#include <AmjuFirst.h>
#include <Game.h>
#include <Timer.h>
#include "CogTestNag.h"
#include "LurkMsg.h"
#include "GameConsts.h"
#include "GSCogTestMenu.h"
#include "ROConfig.h"
#include "GameMode.h"
#include "ChatConsole.h"
#include "GSDoThoseCogTests.h"
#include "GSMain.h"
#include "GSFinishedTests.h"
#include "GSFileUpdateCheck.h"
#include "GSWaitForNewLocation.h"
#include "GSStartGame.h"
#include <AmjuFinal.h>

namespace Amju
{
static float cogtesttime = 0;
static bool nagdone = false;
static int declines = 0;
static int stops = 0;

int GetNumCogTests()
{
  // 3 * stroop, RT, 2 * TM, 2 questionnaire
  return 8; // TODO CONFIG -- or get from login info
}

void NagReset()
{
  cogtesttime = 0;
  nagdone = false;
  declines = 0;
  stops = 0;
}

void OnYesCogTests(GuiElement*)
{
  TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
}

void OnDeclineCogTests(GuiElement*)
{
  // TODO Log number of declines for research
  declines++;

  std::string text;
  if (stops == 0)
  {
    text = "OK, but please do the tests soon! I will ask you again in a few minutes.";
  }
  else
  {
    text = "OK, but please continue with the tests soon! I will ask you again in a few minutes.";
  }
  LurkMsg lm(text, LURK_FG, LURK_BG, AMJU_CENTRE);

  TheLurker::Instance()->Queue(lm);
  nagdone = false;
  cogtesttime = 0;
}

void OnCogTestsAllDone()
{
  GameMode gm = GetGameMode();
  if (gm == AMJU_MODE_NO_GAME || gm == AMJU_MODE_NONE)
  {
    TheGame::Instance()->SetCurrentState(TheGSFinishedTests::Instance());
  }
  else
  {
    TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());
  }
  TheGSCogTestMenu::Instance()->ResetTests();
}

void OnCogTestStopPartWayThrough()
{
  GameMode gm = GetGameMode();

  if (gm == AMJU_MODE_NO_GAME || gm == AMJU_MODE_NONE)
  {
    // You can't play the game, you have to finish the tests.
    TheGSDoThoseCogTests::Instance()->SetPrevState(TheGSCogTestMenu::Instance());
    TheGame::Instance()->SetCurrentState(TheGSDoThoseCogTests::Instance());
  }
  else
  {
    // Go back to Main, will nag again later.
    TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());

    // TODO Log number of declines for research
    stops++;

    LurkMsg lm("OK, but please finish taking the tests soon! I will ask you again in a few minutes.", LURK_FG, LURK_BG, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);
    nagdone = false;
    cogtesttime = 0;
  }
}

CogTestNag::CogTestNag()
{
  static const float COG_TEST_NAG_TIME = ROConfig()->GetFloat("cog-test-nag-time", 120.0f);
  static const float COG_TEST_WAIT_TIME = ROConfig()->GetFloat("cog-test-wait-time", 10.0f);
  cogtesttime = COG_TEST_NAG_TIME - COG_TEST_WAIT_TIME;
}

void CogTestNag::Update()
{
  float dt = TheTimer::Instance()->GetDt();

  // Trigger cog tests if scheduled and not done yet this session (well, process run)
  static const float COG_TEST_NAG_TIME = ROConfig()->GetFloat("cog-test-nag-time", 120.0f);

  if (!TheLurker::Instance()->IsDisplayingMsg() &&
      !TheChatConsole::Instance()->IsActive())
  {
    cogtesttime += dt;
  }

  if (!nagdone && cogtesttime > COG_TEST_NAG_TIME && DoCogTests())
  {
    nagdone = true;
    std::string text;
    
    if (stops == 0)
    {
      text = "I would like to give you some tests. Is that OK?"; 
    }
    else
    {
      // Has already stopped tests before
      text = "Please continue to take my tests. Can you do it now?"; 
    }

    TheLurker::Instance()->ShowYesNo(text, LURK_FG, LURK_BG,
      OnDeclineCogTests, OnYesCogTests);
  }
}

}
