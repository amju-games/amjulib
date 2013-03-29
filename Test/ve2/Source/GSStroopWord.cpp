#include <AmjuFirst.h>
#include "GSStroopWord.h"
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
#include "CogTestNag.h"
#include <AmjuFinal.h>

namespace Amju
{
static const float MAX_TIME = 45.0f; // from Malec et al

static void OnStopTest()
{
  OnCogTestStopPartWayThrough();
}

//static void OnDoneButton()
//{
//  TheGSStroopWord::Instance()->Finished();
//}

static void OnLeftButton()
{
  TheGSStroopWord::Instance()->OnLeftRight(true);
}

static void OnRightButton()
{
  TheGSStroopWord::Instance()->OnLeftRight(false);
}

static void OnReset()
{
  TheGSStroopWord::Instance()->ResetTest();
}

void GSStroopWord::SetTest()
{
  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  word->SetVisible(true);
  GuiButton* left = (GuiButton*)GetElementByName(m_gui, "left-button");
  GuiButton* right = (GuiButton*)GetElementByName(m_gui, "right-button");

  // Pick word and wrong word
  static const int NUM_WORDS = 5;
  static const char* WORDS[NUM_WORDS] = { "red", "green", "blue", "yellow", "purple" };

  // We want two indices, which are different. Shuffle list of all the indices and pick the first two.
  int indices[NUM_WORDS];
  for (int i = 0; i < NUM_WORDS; i++)
  {
    indices[i] = i;
  }

  std::random_shuffle(indices, indices + NUM_WORDS);

  std::string goodWord = WORDS[indices[0]]; //"red";
  std::string wrongWord = WORDS[indices[1]]; //"blue"; // TODO TEMP TEST

  word->SetText(goodWord);

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

void GSStroopWord::OnLeftRight(bool isLeftButton)
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

void GSStroopWord::Finished()
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

    TheCogTestResults::Instance()->StoreResult(new Result(AMJU_COG_TEST_STROOP_WORD, "correct", ToString(m_correct)));
    TheCogTestResults::Instance()->StoreResult(new Result(AMJU_COG_TEST_STROOP_WORD, "incorrect", ToString(m_incorrect)));

    TheGSCogTestMenu::Instance()->AdvanceToNextTest();

    TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
  }
}

GSStroopWord::GSStroopWord()
{
  m_maxTime = MAX_TIME;
  m_timer = m_maxTime;
  m_leftIsCorrect = false;
  m_correct = 0;
  m_incorrect = 0;
  m_testName = "Stroop Word";
  m_isFinished = false;
  m_showLurk = true;
}

void GSStroopWord::ResetTest()
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

void GSStroopWord::Update()
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

void GSStroopWord::Draw()
{
  DrawCogTestBg();
}

void GSStroopWord::Draw2d()
{
  GSGui::Draw2d();
}

void GSStroopWord::OnActive()
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
