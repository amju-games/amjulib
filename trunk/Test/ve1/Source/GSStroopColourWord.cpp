#include "GSStroopColourWord.h"
#include <AmjuGL.h>
#include <ReportError.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <GuiComposite.h>
#include <Game.h>
#include <DrawRect.h>
#include <Timer.h>
#include <SoundManager.h>
#include "ROConfig.h"
#include "GSCogTestMenu.h"
#include "CogTestResults.h"
#include "GSMain.h"
#include "LurkMsg.h"
#include "GameConsts.h"
#include "GameMode.h"
#include "GSCogResults.h"
#include "CogTestNag.h"

namespace Amju
{
static const float MAX_TIME = 45.0f; // from Malec et al

static void OnStopTest()
{
  OnCogTestStopPartWayThrough();

  // Go back to Main, will nag again later.
  TheGame::Instance()->SetCurrentState(TheGSMain::Instance());
}

static void OnDoneButton()
{
  TheGSStroopColourWord::Instance()->Finished();
}

static void OnLeftButton()
{
  TheGSStroopColourWord::Instance()->OnLeftRight(true);
}

static void OnRightButton()
{
  TheGSStroopColourWord::Instance()->OnLeftRight(false);
}

static void OnReset()
{
  TheGSStroopColourWord::Instance()->ResetTest();
}

void GSStroopColourWord::SetTest()
{
  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  word->SetVisible(true);
  GuiButton* left = (GuiButton*)GetElementByName(m_gui, "left-button");
  GuiButton* right = (GuiButton*)GetElementByName(m_gui, "right-button");

  static const int NUM_WORDS = 5;
  static const char* WORDS[NUM_WORDS] = { "red", "green", "blue", "yellow", "purple" };
  static const Colour COLOURS[NUM_WORDS] = 
  {
    Colour(1, 0, 0, 1),
    Colour(0, 1, 0, 1),
    Colour(0, 0, 1, 1),
    Colour(1, 1, 0, 1),
    Colour(1, 0, 1, 1),
  };

  // Pick colour for the word. The player must click the button corresponding to the colour,
  //  NOT the word.

  // We want two indices, which are different. Shuffle list of all the indices and pick the first two.
  int indices[NUM_WORDS];
  for (int i = 0; i < NUM_WORDS; i++)
  {
    indices[i] = i;
  }

  std::random_shuffle(indices, indices + NUM_WORDS);

  std::string goodWord = WORDS[indices[0]]; 
  std::string wrongWord = WORDS[indices[1]]; 

  word->SetText(wrongWord); // text is wrong, but colour is correct
  word->SetFgCol(COLOURS[indices[0]]);

  bool isleft = rand() & 1; ////true; // TODO rand
  m_leftIsCorrect = isleft;
  if (m_leftIsCorrect)
  {
    left->SetText(goodWord);
    right->SetText(wrongWord);
  }
  else
  {
    right->SetText(goodWord);
    left->SetText(wrongWord);
  }
}

void GSStroopColourWord::OnLeftRight(bool isLeftButton)
{
  if (isLeftButton == m_leftIsCorrect)
  {
    TheSoundManager::Instance()->PlayWav(ROConfig()->GetValue("sound-cogtest-correct"));
    m_correct++;
  }
  else
  {
    TheSoundManager::Instance()->PlayWav(ROConfig()->GetValue("sound-cogtest-fail"));
    m_incorrect++;
  }

  GuiText* scoreText = (GuiText*)GetElementByName(m_gui, "score");
  std::string s = "Correct: " + ToString(m_correct) + " Incorrect: " + ToString(m_incorrect);
  scoreText->SetText(s);

  // Animate the text off the screen ?

  SetTest();
}

void OnSeeResultsNo()
{
}

void OnSeeResultsYes()
{
  TheGame::Instance()->SetCurrentState(TheGSCogResults::Instance());
}

void GSStroopColourWord::Finished()
{
  m_isFinished = true;

  // Hide GUI
  GuiButton* left = (GuiButton*)GetElementByName(m_gui, "left-button");
  left->SetVisible(false);

  GuiButton* right = (GuiButton*)GetElementByName(m_gui, "right-button");
  right->SetVisible(false);

  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  word->SetVisible(false);

  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");
  done->SetVisible(false);

  if (TheGSCogTestMenu::Instance()->IsPrac())
  {
    std::string str;
    if (m_correct == 0)
    {
      // TODO offer another practice go
      str = "Oh dear, you didn't get any correct! Well, I am sure you will do better this time!";

      LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, OnReset);
      TheLurker::Instance()->Queue(lm);
    }
    else
    {
      TheSoundManager::Instance()->PlayWav("Sound/applause3.wav");

      str = "OK, you got " + ToString(m_correct) +
        " correct! Let's try it for real!";

      LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, OnReset);
      TheLurker::Instance()->Queue(lm);
    }

    TheGSCogTestMenu::Instance()->SetIsPrac(false);
  }
  else
  {
    std::string str;
    if (m_correct == 0)
    {
      str = "Oh dear, you didn't get any correct! Well, I am sure you will do better next time!";
    }
    else
    {
      str = "Well done! You got " + ToString(m_correct) + " correct!";
      TheSoundManager::Instance()->PlayWav("Sound/applause3.wav");
    }
    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);

    TheCogTestResults::Instance()->StoreResult(new Result(AMJU_COG_TEST_STROOP_COLOUR_WORD, "correct", ToString(m_correct)));
    TheCogTestResults::Instance()->StoreResult(new Result(AMJU_COG_TEST_STROOP_COLOUR_WORD, "incorrect", ToString(m_incorrect)));

    TheGSCogTestMenu::Instance()->AdvanceToNextTest();

    // This is the last test. Show final msg.
    // TODO Show results
    //LurkMsg lm2("That's the end. Thank you for doing my tests! Would you like to look at your results?", 
    //  LURK_FG, LURK_BG, AMJU_CENTRE);
    TheLurker::Instance()->ShowYesNo(
      "That's the end. Thank you for doing my tests! Would you like to look at your results?", 
      LURK_FG, LURK_BG, OnSeeResultsNo, OnSeeResultsYes);
    
    SetDoCogTests(false); // TODO Or SetCogTestsDone()

    TheGame::Instance()->SetCurrentState(TheGSMain::Instance());
  }
}

GSStroopColourWord::GSStroopColourWord()
{
  m_maxTime = MAX_TIME;
  m_timer = m_maxTime;
  m_leftIsCorrect = false;
  m_correct = 0;
  m_incorrect = 0;
  m_testName = "Stroop Colour Word";
  m_isFinished = false;
  m_showLurk = true;
}

void GSStroopColourWord::ResetTest()
{
  GuiButton* left = (GuiButton*)GetElementByName(m_gui, "left-button");
  left->SetCommand(Amju::OnLeftButton);
  left->SetVisible(true);

  GuiButton* right = (GuiButton*)GetElementByName(m_gui, "right-button");
  right->SetCommand(Amju::OnRightButton);
  right->SetVisible(true);

  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  word->SetVisible(true);

  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");
  done->SetCommand(Amju::OnStopTest);
  done->SetVisible(true);

  m_timer = m_maxTime;
  m_leftIsCorrect = false;
  m_correct = 0;
  m_incorrect = 0;
  m_isFinished = false;

  GuiText* scoreText = (GuiText*)GetElementByName(m_gui, "score");
  std::string s = "Correct: " + ToString(m_correct) + " Incorrect: " + ToString(m_incorrect);
  scoreText->SetText(s);
}

void GSStroopColourWord::Update()
{
  GSGui::Update();

  m_timer -= TheTimer::Instance()->GetDt();

  GuiText* timeText = (GuiText*)GetElementByName(m_gui, "timer");
  std::string s;
  if (!m_isFinished)
  {
    if (m_timer > 0)
    {
      int min = (int)(m_timer / 60.0f);
      int sec = (int)(m_timer - 60.0f * min);
      s = ToString(min) + ":" + (sec < 10 ? "0" : "") + ToString(sec);
    }
    else
    {
      // TODO flash
      s = "0:00";

      Finished();
    }

    timeText->SetText(s);
  }
}

void GSStroopColourWord::Draw()
{
  DrawCogTestBg();
}

void GSStroopColourWord::Draw2d()
{
  GSGui::Draw2d();
}

void GSStroopColourWord::OnActive()
{
  GSBase::OnActive();

  LoadCogTestBg();

  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));

  m_gui = LoadGui("gui-stroop-word.txt"); 
  Assert(m_gui);

  ResetTest();

  SetTest(); // set first word
}

} // namespace
