#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Screen.h>
#include <Game.h>
#include <GuiButton.h>
#include <SoundManager.h>
#include "GSCogTestMenu.h"
#include "GSFileUpdateCheck.h"
#include "GSLetterCancellation.h"
#include "GSTitle.h"
#include "GSStroopWord.h"
#include "GSStroopColour.h"
#include "GSStroopColourWord.h"
#include "GSReactionTime.h"
#include "GSTrailMakingTest.h"
#include "CogTestResults.h"
#include "Ve1Character.h"
#include "Ve1SceneGraph.h"
#include "LurkMsg.h"
#include "GameMode.h"
#include "ROConfig.h"
#include "GameConsts.h"
#include "GSFinishedTests.h"
#include "CogTestNag.h"
#include "ObjectUpdater.h"
#include <AmjuFinal.h>

namespace Amju
{
/*
static void OnCancelButton()
{
  // If we cancel, the session will not be complete so we should have to do the cog tests
  //  again ? 
  // No, the participant just skips the test, if that's what they want to do.

  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
  TheGSCogTestMenu::Instance()->ResetTests();
}
*/

static void NoPrac()
{
  TheGSCogTestMenu::Instance()->DoPractice(false);
}

static void YesPrac()
{
  TheGSCogTestMenu::Instance()->DoPractice(true);
}

static void AskPractice()
{
  TheGSCogTestMenu::Instance()->AskPractice();
}

static void LetterSigmaTest()
{
  TheGSLetterCancellation::Instance()->LoadConfig(AMJU_COG_TEST_SYMBOL_CAN, "letter-cancel-sigma.txt");
  TheGame::Instance()->SetCurrentState(TheGSLetterCancellation::Instance());
}

static void LetterMTest()
{
  TheGSLetterCancellation::Instance()->LoadConfig(AMJU_COG_TEST_LETTER_CAN, "letter-cancel-m.txt");
  TheGame::Instance()->SetCurrentState(TheGSLetterCancellation::Instance());
}

static void StroopWord()
{
  TheGame::Instance()->SetCurrentState(TheGSStroopWord::Instance());
}

static void StroopColour()
{
  TheGame::Instance()->SetCurrentState(TheGSStroopColour::Instance());
}

static void StroopColourWord()
{
  TheGame::Instance()->SetCurrentState(TheGSStroopColourWord::Instance());
}

static void ReactionTime()
{
  TheGame::Instance()->SetCurrentState(TheGSReactionTime::Instance());
}

static void TrailMakingTest()
{
  TheGame::Instance()->SetCurrentState(TheGSTrailMakingTest::Instance());
}

static void Done()
{
  OnCogTestsAllDone();
}

GSCogTestMenu::GSCogTestMenu()
{
  ResetTests();
  m_func = 0;
  m_isPrac = false;
  m_showLurk = true;
}

void GSCogTestMenu::ResetTests()
{
  m_nextTest = ROConfig()->GetInt("cogtest-first-test", 0); // For testing
}

void GSCogTestMenu::Update()
{
  GSGui::Update();
  TheObjectUpdater::Instance()->Update();
  UpdateHeartCount();
}

void GSCogTestMenu::Draw()
{
  DrawCogTestBg();
}

void GSCogTestMenu::Draw2d()
{
  GSGui::Draw2d();
}

void GSCogTestMenu::AdvanceToNextTest()
{
  m_nextTest++;
}

bool GSCogTestMenu::IsDoingTests()
{
  bool b = DoCogTests(); //// && m_nextTest < 6; // Why < 6??
  return b;
}

void GSCogTestMenu::OnActive()
{
  // TODO Fade vol down to v quiet
//  TheSoundManager::Instance()->StopSong();
//  TheSoundManager::Instance()->PlayWav(ROConfig()->GetValue("sound-new-cogtest"));

  //GSGui::OnActive();
  GSBase::OnActive();
  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));
  LoadCogTestBg();

  m_gui = LoadGui("gui-cogtestmenu.txt");
  Assert(m_gui);

  // Set text and OK button command depending on next test to perform...
  // TODO Use localisation system so we can improve all the text.

  m_isPrac = false;
  std::string str;

  // Check for tests already done (may have restarted exe)
  Time today = Time::Now();

  // This is the list of cog tests to take, in order. Not necessarily all the tests, and not necessarily in
  //  the order they are enumerated in TestId.
  // TODO This could be read from the server, so each group could have a custom set of tests.
  TestId LOOKUP_TEST_ID[] = 
  {
    AMJU_COG_TEST_REACTION_TIME,
    AMJU_COG_TEST_LETTER_CAN,
    AMJU_COG_TEST_SYMBOL_CAN,
    AMJU_COG_TEST_STROOP_COLOUR,
    AMJU_COG_TEST_STROOP_WORD,
    AMJU_COG_TEST_STROOP_COLOUR_WORD,
    AMJU_COG_TEST_TRAIL_MAKING,
    AMJU_COG_TEST_TRAIL_MAKING_ALTERNATE,

    AMJU_COG_TEST_MAX
  };

  CogTestResults* ctr = TheCogTestResults::Instance();
  while (LOOKUP_TEST_ID[m_nextTest] != AMJU_COG_TEST_MAX  && 
         ctr->IsTestComplete(today, LOOKUP_TEST_ID[m_nextTest]))
  {
    std::cout << "Test " << LOOKUP_TEST_ID[m_nextTest] << " is done!\n";
    m_nextTest++;
  }

  switch (LOOKUP_TEST_ID[m_nextTest])
  {
  case AMJU_COG_TEST_REACTION_TIME:
    str = "OK, thanks! This first one is a reaction time test. Please press the big red button as soon as the alarm goes off!";
    m_func = Amju::ReactionTime;
    break;

  case AMJU_COG_TEST_LETTER_CAN:
    str = "This next one is called a letter cancellation test. Please click on all the M letters that you see. You have 3 minutes to complete this task.";
    m_func = Amju::LetterMTest;
    break;

  case AMJU_COG_TEST_SYMBOL_CAN:
    {
    str = "This time, click on all the sigma letters that you see. They look like this..."; 
    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);

    GuiText* text = new GuiText;
    text->SetFont("font2d/symbol-font.font");
    text->SetTextSize(4.5f); // TODO CONFIG
    text->SetSize(Vec2f(1.6f, 0.5f)); 
    text->SetText("S");
    text->SizeToText();
    text->SetFgCol(LURK_FG);
    lm.Set(text, LURK_FG, LURK_BG, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);

    str = "It's the same rules as before, but this time find the sigma letters!";

    m_func = Amju::LetterSigmaTest;
    }
    break;

  case AMJU_COG_TEST_STROOP_COLOUR:
    str = "This next one is called a Stroop Colour test. I will show you a coloured rectangle. Please click on the button which says the colour of the rectangle.";
    m_func = Amju::StroopColour;
    break;

  case AMJU_COG_TEST_STROOP_WORD:
    str = "This is called the Stroop Word test. I will show you a word. Please click on the button which is the same as the word. Easy!";
    m_func = Amju::StroopWord;
    break;

  case AMJU_COG_TEST_STROOP_COLOUR_WORD:
    {
      str = "This is called the Stroop Colour Word test.";
      LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE);
      TheLurker::Instance()->Queue(lm);
    }
    str = "I will show you a word like before. But this time, the words are coloured. "
      "You have to click the button for the colour, not the word. It's quite tricky!";
    m_func = Amju::StroopColourWord;
    break;

  case AMJU_COG_TEST_TRAIL_MAKING:
    str = "This is called a trail making test.";
    m_func = Amju::TrailMakingTest;
    TheGSTrailMakingTest::Instance()->SetIsAlternatingVersion(false);
    break;

  case AMJU_COG_TEST_TRAIL_MAKING_ALTERNATE:
    str = "This is called a trail making test (alternating).";
    m_func = Amju::TrailMakingTest;
    TheGSTrailMakingTest::Instance()->SetIsAlternatingVersion(true);
    break;

  case AMJU_COG_TEST_MAX:
    str = "Well done and thank you for doing all those tests!";
    m_func = Amju::Done;
    break;

  default:
    Assert(0);
  }

  if (LOOKUP_TEST_ID[m_nextTest] == AMJU_COG_TEST_MAX)
  {
    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, Amju::Done);
    TheLurker::Instance()->Queue(lm);
  }
  else
  {
    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, Amju::AskPractice);
    TheLurker::Instance()->Queue(lm);
  }
}

void GSCogTestMenu::AskPractice()
{
  std::string q("Would you like to have a practice go first ?");
  TheLurker::Instance()->ShowYesNo(q, LURK_FG, LURK_BG, Amju::NoPrac, Amju::YesPrac);
}

void GSCogTestMenu::SetIsPrac(bool b)
{
  m_isPrac = b;
}

bool GSCogTestMenu::IsPrac() const
{
  return m_isPrac;
}

void GSCogTestMenu::DoPractice(bool b)
{
  if (b)
  {
    SetIsPrac(b);
  }
  m_func();
}

} // namespace
