#include <AmjuFirst.h>
#include "GSReactionTime.h"
#include <AmjuGL.h>
#include <ReportError.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <GuiComposite.h>
#include <Game.h>
#include <DrawRect.h>
#include <Timer.h>
#include <SoundManager.h>
#include "GSCogTestMenu.h"
#include "CogTestResults.h"
#include "LurkMsg.h"
#include "GSMain.h"
#include "ROConfig.h"
#include "GameConsts.h"
#include "CogTestNag.h"
#include "ObjectUpdater.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnButton()
{
  TheGSReactionTime::Instance()->OnButton();
}

static void NextGo()
{
  TheGSReactionTime::Instance()->NextGo();
}

void StartRT()
{
  TheGSReactionTime::Instance()->SetTest();
}

GSReactionTime::GSReactionTime()
{
  m_reactionTime = 0; 
  m_waitTime = 0; 
  m_maxWaitTime = 0; 
  m_testNum = 0; 
  m_maxTestNum = 3;
  m_mode = RT_NOT_STARTED_YET;
  m_showLurk = true;
  m_numBadClicks = 0;
}

void GSReactionTime::OnButton()
{
  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");
  done->SetVisible(false);

  switch (m_mode)
  {
  case RT_TIMING:
    {
std::cout << "Reaction time result: " << m_reactionTime << "s\n";
    std::string str = "Well done! You hit the button in " + 
      ToString(m_reactionTime, 2) +
      " seconds!";

    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);

    m_mode = RT_CONTINUE;
    GuiButton* b = (GuiButton*)GetElementByName(m_gui, "button");
    b->SetIsEnabled(false); 
    b->SetText("");
      
    GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
    word->SetText(ToString(m_reactionTime, 2));

    TheSoundManager::Instance()->PlayWav("sound/applause3.wav");

    if (TheGSCogTestMenu::Instance()->IsPrac())
    {
      m_testNum = -1; // so practice goes not counted
      TheGSCogTestMenu::Instance()->SetIsPrac(false);
      LurkMsg lm("That was a practice. Now let's try for real!", 
        LURK_FG, LURK_BG, AMJU_CENTRE, Amju::NextGo);
      TheLurker::Instance()->Queue(lm);
      // TODO Ask if player would like another prac
    }
    else
    {
      // Send results
      // Store as ms, not seconds, so all result types are int.
      int rtMs = (int)(m_reactionTime * 1000.0f);
      TheCogTestResults::Instance()->StoreResult(new Result(
        AMJU_COG_TEST_REACTION_TIME, "time" + ToString(m_testNum), ToString(rtMs)));

      // Send number of bad clicks
      TheCogTestResults::Instance()->StoreResult(new Result(
        AMJU_COG_TEST_REACTION_TIME, "bad" + ToString(m_testNum), ToString(m_numBadClicks)));
      
      NextGo();
    }
    }
    break;

  case RT_WAITING:
    // If mouse button pressed before "GO" displayed, player might be spamming the mouse button.
    // Reset wait period - TODO sound/visual to show that mouse click is ignored.
    TheSoundManager::Instance()->PlayWav(ROConfig()->GetValue("sound-cogtest-fail"));

    // TODO Store the number of these "Bad" clicks as a result.
    m_numBadClicks++;

    m_waitTime = 0;
    break;

  default:
    // TODO restart timer
    Assert(0);
  }
}

void GSReactionTime::Quit()
{
  // TODO Play a sound
  OnCogTestStopPartWayThrough();
}

void GSReactionTime::NextGo()
{
  Assert(m_mode == RT_CONTINUE);

  // Bad clicks score is for each round of the test
  m_numBadClicks = 0;

  m_testNum++;
  if (m_testNum == m_maxTestNum)
  {
    LurkMsg lm("Thanks for doing my reaction time test!", 
      LURK_FG, LURK_BG, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);

    m_correct = 3; // so added to score
    UpdateScore();

    TheGSCogTestMenu::Instance()->AdvanceToNextTest();

    TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
  }
  else
  {
    if (m_testNum > 0)
    {
      std::string str;
      // TODO Switch, different phrase each time ?
      if (m_testNum == m_maxTestNum - 1)
      {
        str = "This is your last go!";
      }
      else
      {
        str = "That was go number " + 
          ToString(m_testNum) + 
          " of " + 
          ToString(m_maxTestNum) + 
          ". Try again, and see if you can beat your time!";
      }
      // Don't continue with test until msg OKed
      LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, StartRT);
      TheLurker::Instance()->Queue(lm);
    }
    else
    {
      SetTest(); // no msg, so start immediately
    }
  }
}

void GSReactionTime::Update()
{
  GSCogTestBase::Update();
  TheObjectUpdater::Instance()->Update(); //?
  UpdateHeartCount();

  PointPracArrow(Vec2f(2.0f, 2.0f)); 

  float dt = TheTimer::Instance()->GetDt();

  GuiButton* b = (GuiButton*)GetElementByName(m_gui, "button");
  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  GuiText* timeText = (GuiText*)GetElementByName(m_gui, "timer");

  switch (m_mode)
  {
  case RT_NOT_STARTED_YET:
    break;

  case RT_BEFORE_TEST:
    m_waitTime += dt;
    if (m_waitTime >= 3.0f) // TODO TEMP TEST
    {
      m_waitTime = 0;
      word->SetText("");
      m_mode = RT_WAITING;

      //TheSoundManager::Instance()->PlayWav("sound/ticktock.wav");
    }
    break;

  case RT_WAITING:
    m_waitTime += dt;
    if (m_waitTime >= m_maxWaitTime)
    {
      m_waitTime = 0;
      word->SetText("GO!");
      b->SetText("GO!");
      m_mode = RT_TIMING;
      b->SetIsEnabled(true); 

      TheSoundManager::Instance()->PlayWav(ROConfig()->GetValue("sound-rt-go"));
    }
    break;

  case RT_TIMING:
    PointPracArrow(Vec2f(0.2f, -0.2f)); // TODO CONFIG
    m_reactionTime += dt;
    timeText->SetText(ToString(m_reactionTime, 2));

    word->SetText(ToString(m_reactionTime, 2));

    break;

  case RT_CONTINUE:
    // Wait for button press
    break;
  }
}

void GSReactionTime::Draw()
{
  DrawCogTestBg();
}

void GSReactionTime::Draw2d()
{
  GSGui::Draw2d();
}

void GSReactionTime::Finished()
{
  // Player has run out of time - move on..

}

void GSReactionTime::OnActive()
{
//  GSGui::OnActive();
  GSBase::OnActive();

  LoadCogTestBg();

  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));

  m_gui = LoadGui("gui-reactiontime.txt");
  Assert(m_gui);
  LoadCommonGui();

  // Set focus element, cancel element, command handlers
  GuiButton* b = (GuiButton*)GetElementByName(m_gui, "button");

  // Execute handler when button pressed down, not when released!
  b->SetOnPressedDownFunc(Amju::OnButton);

  b->SetHasFocus(true);
  b->SetShowIfFocus(false);
  b->SetIsEnabled(false); 
  b->SetText("");

  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  word->SetText("");

  m_testNum = 0; 

  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");
  done->SetVisible(false);

  std::string str = "OK, when I say GO, press the button as quickly as you can!";

  LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, StartRT);
  TheLurker::Instance()->Queue(lm);

  m_mode = RT_NOT_STARTED_YET;
}

void GSReactionTime::SetTest()
{
std::cout << "Reaction time, test number: " << m_testNum << "\n";
  GuiButton* b = (GuiButton*)GetElementByName(m_gui, "button");
  b->SetIsEnabled(false); 
  b->SetText("");

  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  word->SetText("ready...");

  float minTime = ROConfig()->GetFloat("rt-min-wait-time", 2.0);
  float extra = ROConfig()->GetFloat("rt-extra-wait-time", 2.0);
  m_maxWaitTime = minTime + (float)rand() / (float)RAND_MAX * extra; // wait between min and min+extra

std::cout << "Waiting time is: " << m_maxWaitTime << "s\n";

  m_mode = RT_BEFORE_TEST;
  m_waitTime = 0;
  m_reactionTime = 0;

  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");
  done->SetVisible(true);
}

} // namespace
