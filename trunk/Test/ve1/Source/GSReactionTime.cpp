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

namespace Amju
{
// TODO CONFIG
static const Colour FG_COLOUR(1, 1, 1, 1);
static const Colour BG_COLOUR(0.5f, 0, 0.5f, 0.5f);

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

void OnDoneButton()
{
  TheGSReactionTime::Instance()->Quit();
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

    LurkMsg lm(str, FG_COLOUR, BG_COLOUR, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);

    m_mode = RT_CONTINUE;
    GuiButton* b = (GuiButton*)GetElementByName(m_gui, "button");
    b->SetIsEnabled(false); 
    b->SetText("");
      
    GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
    word->SetText(ToString(m_reactionTime, 2));

    TheSoundManager::Instance()->PlayWav("Sound/applause3.wav");

    if (TheGSCogTestMenu::Instance()->IsPrac())
    {
      m_testNum = -1; // so practice goes not counted
      TheGSCogTestMenu::Instance()->SetIsPrac(false);
      LurkMsg lm("That was a practice. Now let's try for real!", 
        FG_COLOUR, BG_COLOUR, AMJU_CENTRE, Amju::NextGo);
      TheLurker::Instance()->Queue(lm);
      // TODO Ask if player would like another prac
    }
    else
    {
      // Send results
      TheCogTestResults::Instance()->StoreResult(new Result(
        AMJU_COG_TEST_REACTION_TIME, "time" + ToString(m_testNum), ToString(m_reactionTime)));

      NextGo();
    }
    }
    break;

  default:
    // TODO restart timer
    Assert(0);
  }
}

void GSReactionTime::Quit()
{
  // TODO Play a sound

  TheGSCogTestMenu::Instance()->AdvanceToNextTest();

  // Go back to Main, to collect rewards, then go back (NPC controls this)
  TheGame::Instance()->SetCurrentState(TheGSMain::Instance());
}

void GSReactionTime::NextGo()
{
  Assert(m_mode == RT_CONTINUE);

  m_testNum++;
  if (m_testNum == m_maxTestNum)
  {
    LurkMsg lm("Thanks for playing my reaction time game! I have got some other things for you to do!", 
      FG_COLOUR, BG_COLOUR, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);

    TheGSCogTestMenu::Instance()->AdvanceToNextTest();

    // Go back to Main, to collect rewards, then go back (NPC controls this)
    TheGame::Instance()->SetCurrentState(TheGSMain::Instance());
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
      LurkMsg lm(str, FG_COLOUR, BG_COLOUR, AMJU_CENTRE, StartRT);
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
  GSGui::Update();

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

      //TheSoundManager::Instance()->PlayWav("Sound/ticktock.wav");
    }
    break;

  case RT_WAITING:
    m_waitTime += dt;
    if (m_waitTime >= m_maxWaitTime)
    {
      m_waitTime = 0;
      word->SetText("GO!");
      b->SetText("click me!");
      m_mode = RT_TIMING;
      b->SetIsEnabled(true); 

      TheSoundManager::Instance()->PlayWav(ROConfig()->GetValue("sound-rt-go"));
    }
    break;

  case RT_TIMING:
    m_reactionTime += dt;
    timeText->SetText(ToString(m_reactionTime, 2));
    break;

  case RT_CONTINUE:
    // Wait for button press
    break;
  }
}

void GSReactionTime::Draw()
{
}

void GSReactionTime::Draw2d()
{
  GSGui::Draw2d();
}

void GSReactionTime::OnActive()
{
//  GSGui::OnActive();

  GSBase::OnActive();
  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));

  m_gui = LoadGui("gui-reactiontime.txt");
  Assert(m_gui);

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
  done->SetCommand(Amju::OnDoneButton);
  done->SetVisible(false);

  std::string str = "OK, when I say GO, press the button as quickly as you can!";

  LurkMsg lm(str, FG_COLOUR, BG_COLOUR, AMJU_CENTRE, StartRT);
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

  m_maxWaitTime = 4.0f + (float)rand() / (float)RAND_MAX * 3.0f; // TODO Rand between ? and 7.0
std::cout << "Waiting time is: " << m_maxWaitTime << "s\n";

  m_mode = RT_BEFORE_TEST;
  m_waitTime = 0;
  m_reactionTime = 0;

  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");
  done->SetVisible(true);
}

} // namespace
