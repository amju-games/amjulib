#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <ReportError.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <GuiComposite.h>
#include <Game.h>
#include <DrawRect.h>
#include <Timer.h>
#include <SoundManager.h>
#include "GSStroopBase.h"
#include "ROConfig.h"
#include "GSCogTestMenu.h"
#include "CogTestResults.h"
#include "GSMain.h"
#include "LurkMsg.h"
#include "GameConsts.h"
#include "ObjectUpdater.h"
#include "CogTestNag.h"
#include "HeartCount.h"
#include <AmjuFinal.h>

namespace Amju
{
static const float MAX_TIME = 45.0f; // from Malec et al

static void OnReset()
{
  GameState* gs = TheGame::Instance()->GetState();
  GSStroopBase* sb = dynamic_cast<GSStroopBase*>(gs);
  Assert(sb);
  if (sb)
  {
    sb->ResetTest();
  }
}

static void OnStopTest()
{
  OnCogTestStopPartWayThrough(); // global function, right?
}

class CommandChoiceButton : public GuiCommand
{
public:
  CommandChoiceButton(int choice) : m_choice(choice) {}
  virtual bool Do()
  {
    GameState* gs = TheGame::Instance()->GetState();
    GSStroopBase* sb = dynamic_cast<GSStroopBase*>(gs);
    Assert(sb);
    if (sb)
    {
      sb->OnChoiceButton(m_choice);
    }

    return false;
  }

private:
  int m_choice;
};

const char* GSStroopBase::WORDS[GSStroopBase::NUM_WORDS] = { "red", "green", "blue", "yellow", "purple" };

const Colour GSStroopBase::COLOURS[GSStroopBase::NUM_WORDS] = 
{
  Colour(1, 0, 0, 1),
  Colour(0, 1, 0, 1),
  Colour(0, 0, 1, 1),
  Colour(1, 1, 0, 1),
  Colour(1, 0, 1, 1),
};

GSStroopBase::GSStroopBase()
{
  m_maxTime = MAX_TIME;
  m_timer = m_maxTime;
  m_correctChoice = -1;
  m_correct = 0;
  m_incorrect = 0;
  m_isFinished = false;
  m_showLurk = true;

  m_testId = -1; // MUST SET IN SUBCLASS CTOR

  for (int i = 0; i < NUM_WORDS; i++)
  {
    m_indices[i] = i;
  }
}

void GSStroopBase::ResetTest()
{
  for (int i = 0; i < 4; i++)
  {
    GuiButton* button = (GuiButton*)GetElementByName(m_gui, "button" + ToString(i + 1));
    button->SetCommand(new CommandChoiceButton(i));
    button->SetVisible(true);
  }
 
  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  word->SetVisible(true);

  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");
  done->SetCommand(Amju::OnStopTest);
  done->SetVisible(true);

  m_timer = m_maxTime;
  m_correctChoice = -1;
  m_correct = 0;
  m_incorrect = 0;
  m_isFinished = false;

  GuiText* scoreText = (GuiText*)GetElementByName(m_gui, "score");
  std::string s = "Correct: " + ToString(m_correct) + " Incorrect: " + ToString(m_incorrect);
  scoreText->SetText(s);
}

void GSStroopBase::SetTest()
{
  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  word->SetVisible(true);

  std::random_shuffle(m_indices, m_indices + NUM_WORDS);

  m_correctChoice = rand() % 4; 

  for (int i = 0; i < 4; i++)
  {
    GuiButton* button = (GuiButton*)GetElementByName(m_gui, "button" + ToString(i + 1));
    button->SetText(WORDS[m_indices[i]]);
  }

  word->SetText(WORDS[m_indices[m_correctChoice]]);
}

void GSStroopBase::OnActive()
{
  GSBase::OnActive();

  LoadCogTestBg();

  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));

  m_gui = LoadGui("gui-stroop.txt"); // same for all Stroop tests
  Assert(m_gui);

  ResetTest();

  SetTest(); // set first word
}

void GSStroopBase::Update()
{
  GSGui::Update();
  TheObjectUpdater::Instance()->Update();
  UpdateHeartCount();

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

void GSStroopBase::Draw()
{
  DrawCogTestBg();
}

void GSStroopBase::Draw2d()
{
  GSGui::Draw2d();
}

void GSStroopBase::OnChoiceButton(int choice)
{
  Assert(m_correctChoice != -1);

  if (choice == m_correctChoice)
  {
    // TODO How much score improvement?
    ChangePlayerCount(SCORE_KEY, 100); 
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

void GSStroopBase::Finished()
{
  m_isFinished = true;

  // Hide GUI
  for (int i = 0; i < 4; i++)
  {
    GuiButton* button = (GuiButton*)GetElementByName(m_gui, "button" + ToString(i + 1));
    button->SetVisible(false);
  }

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

    Assert(m_testId != -1);
    TheCogTestResults::Instance()->StoreResult(new Result(m_testId, "correct", ToString(m_correct)));
    TheCogTestResults::Instance()->StoreResult(new Result(m_testId, "incorrect", ToString(m_incorrect)));

    TheGSCogTestMenu::Instance()->AdvanceToNextTest();

    TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
  }
}

}
