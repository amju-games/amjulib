#include <Game.h>
#include <Timer.h>
#include "CogTestNag.h"
#include "LurkMsg.h"
#include "GameConsts.h"
#include "GSCogTestMenu.h"
#include "ROConfig.h"
#include "GameMode.h"

namespace Amju
{
static float cogtesttime = 0;
static bool nagdone = false;

void OnYesCogTests()
{
  TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
}

void OnNoCogTests()
{
  // TODO Log number of declines for research

  LurkMsg lm("OK, but please do the tests soon! I will ask you again in a few minutes.", LURK_FG, LURK_BG, AMJU_CENTRE);
  TheLurker::Instance()->Queue(lm);
  nagdone = false;
  cogtesttime = 0;
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
  //static bool cogtestsdone = false;
  static const float COG_TEST_NAG_TIME = ROConfig()->GetFloat("cog-test-nag-time", 120.0f);
  //static const float COG_TEST_WAIT_TIME = ROConfig()->GetFloat("cog-test-wait-time", 10.0f);
  //static float cogtesttime = COG_TEST_NAG_TIME - COG_TEST_WAIT_TIME;

  if (!TheLurker::Instance()->IsDisplayingMsg())
  {
    cogtesttime += dt;
  }

  if (!nagdone && cogtesttime > COG_TEST_NAG_TIME && DoCogTests())
  {
    nagdone = true;
    std::string text = 
      "I would like to make sure you are OK after the crash. So please allow me to give you some tests. Is that OK ?"; 
    TheLurker::Instance()->ShowYesNo(text, LURK_FG, LURK_BG,
      OnNoCogTests, OnYesCogTests);
  }
}

}