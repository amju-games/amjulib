#include "GSStroopWord.h"
#include <AmjuGL.h>
#include <ReportError.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <GuiComposite.h>
#include <Game.h>
#include <DrawRect.h>
#include <Timer.h>
#include "GSCogTestMenu.h"
#include "CogTestResults.h"


namespace Amju
{
static const float MAX_TIME = 45.0f; // from Malec et al

static void OnLeftButton()
{
  TheGSStroopWord::Instance()->OnLeftRight(true);
}

static void OnRightButton()
{
  TheGSStroopWord::Instance()->OnLeftRight(false);
}

void GSStroopWord::SetTest()
{
  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
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
    // TODO sound
    m_correct++;
  }
  else
  {
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
  // TODO Send results
std::cout << "Finished! Sending results to server...\n";

  TheCogTestResults::Instance()->StoreResult(Result(m_testName, "correct", ToString(m_correct)));
  TheCogTestResults::Instance()->StoreResult(Result(m_testName, "incorrect", ToString(m_incorrect)));
//  TheCogTestResults::Instance()->StoreResult(Result(m_testName, "time", ToString(m_timer)));

  // TODO Where should we go when we administer the test for real ?
  TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
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
}

void GSStroopWord::Update()
{
  GSGui::Update();

  m_timer -= TheTimer::Instance()->GetDt();

  GuiText* timeText = (GuiText*)GetElementByName(m_gui, "timer");
  std::string s;
  if (m_timer > 0)
  {
    int min = (int)(m_timer / 60.0f);
    int sec = (int)(m_timer - 60.0f * min);
    s = ToString(min) + ":" + (sec < 10 ? "0" : "") + ToString(sec);
  }
  else
  {
    m_isFinished = true;
    // TODO flash
    s = "0:00";

    Finished();
  }

  timeText->SetText(s);

}

void GSStroopWord::Draw()
{
//  GSGui::Draw();

}

void GSStroopWord::Draw2d()
{
  GSGui::Draw2d();
}

void GSStroopWord::OnActive()
{
//  GSGui::OnActive();

  GSBase::OnActive();
  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));

  m_gui = LoadGui("gui-stroop-word.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
  GuiButton* left = (GuiButton*)GetElementByName(m_gui, "left-button");
  left->SetCommand(Amju::OnLeftButton);
  //left->SetHasFocus(true);

  GuiButton* right = (GuiButton*)GetElementByName(m_gui, "right-button");
  right->SetCommand(Amju::OnRightButton);
//  cancel->SetIsCancelButton(true);

  m_timer = m_maxTime;;
  m_leftIsCorrect = false;
  m_correct = 0;
  m_incorrect = 0;
  m_isFinished = false;

  SetTest(); // set first word
}

} // namespace
