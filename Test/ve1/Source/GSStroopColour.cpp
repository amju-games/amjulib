#include "GSStroopColour.h"
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
  TheGSStroopColour::Instance()->OnLeftRight(true);
}

static void OnRightButton()
{
  TheGSStroopColour::Instance()->OnLeftRight(false);
}

void GSStroopColour::SetTest()
{
////  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
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

  std::string good = WORDS[indices[0]]; 
  std::string wrong = WORDS[indices[1]]; 

  //word->SetText(wrong); // text is wrong, but colour is correct
  //word->SetFgCol(COLOURS[indices[0]]);

  m_goodColour = COLOURS[indices[0]];

  bool isleft = rand() & 1; ////true; // TODO rand
  m_leftIsCorrect = isleft;
  if (m_leftIsCorrect)
  {
    left->SetText(good);
    right->SetText(wrong);
  }
  else
  {
    right->SetText(good);
    left->SetText(wrong);
  }
}

void GSStroopColour::OnLeftRight(bool isLeftButton)
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

void GSStroopColour::Finished()
{
  // TODO Send results
std::cout << "Finished! Sending results to server...\n";

  TheCogTestResults::Instance()->StoreResult(new Result(AMJU_COG_TEST_STROOP_COLOUR, "correct", ToString(m_correct)));
  TheCogTestResults::Instance()->StoreResult(new Result(AMJU_COG_TEST_STROOP_COLOUR, "incorrect", ToString(m_incorrect)));
//  TheCogTestResults::Instance()->StoreResult(new Result(AMJU_COG_TEST_STROOP_COLOUR, "time", ToString(m_timer)));

  // TODO Where should we go when we administer the test for real ?
  TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
}

GSStroopColour::GSStroopColour()
{
  m_maxTime = MAX_TIME;
  m_timer = m_maxTime;
  m_leftIsCorrect = false;
  m_correct = 0;
  m_incorrect = 0;
  m_testName = "Stroop Colour ";
  m_isFinished = false;
  m_showLurk = true;
}

void GSStroopColour::Update()
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

void GSStroopColour::Draw()
{
//  GSGui::Draw();

}

void GSStroopColour::Draw2d()
{
  // Draw coloured rect
  AmjuGL::PushAttrib(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  PushColour();
  Rect r(-0.5f, 0.5f, -0.2f, 0.2f);
  AmjuGL::SetColour(m_goodColour);
  DrawSolidRect(r);  
  PopColour();
  AmjuGL::PopAttrib();

  GSGui::Draw2d();
}

void GSStroopColour::OnActive()
{
//  GSGui::OnActive();

  GSBase::OnActive();
  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));

  m_gui = LoadGui("gui-stroop-word.txt"); // Same GUI?!
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
  GuiButton* left = (GuiButton*)GetElementByName(m_gui, "left-button");
  left->SetCommand(Amju::OnLeftButton);
  //left->SetHasFocus(true);

  GuiButton* right = (GuiButton*)GetElementByName(m_gui, "right-button");
  right->SetCommand(Amju::OnRightButton);
//  cancel->SetIsCancelButton(true);

  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  word->SetVisible(false);

  m_timer = m_maxTime;;
  m_leftIsCorrect = false;
  m_correct = 0;
  m_incorrect = 0;
  m_isFinished = false;

  SetTest(); // set first word
}

} // namespace
