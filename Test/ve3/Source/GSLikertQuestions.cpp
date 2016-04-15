#include <Game.h>
#include <GuiText.h>
#include "GSLikertQuestions.h"
#include "GSCogTestMenu.h"
#include "CogTestResults.h"

namespace Amju
{
class LikertButtonCommand : public GuiCommand
{
public:
  LikertButtonCommand(int i) : m_button(i) {}
  bool Do() override
  {
    GameState* gs = TheGame::Instance()->GetState();
    GSLikertQuestions* glq = dynamic_cast<GSLikertQuestions*>(gs);
    Assert(glq);
    glq->OnChoiceButton(m_button);
    return false;
  }

private:
  int m_button;
};

void GSLikertQuestions::SetQuestion()
{
  Assert(m_currentQuestion < (int)m_questions.size());
  std::string q = m_questions[m_currentQuestion];
  GuiText* text = (GuiText*)GetElementByName(m_gui, "question-text");
  Assert(text);
  text->SetText(q);

  std::string s = "This is question " + ToString(m_currentQuestion + 1) +
    " out of " + ToString((int)m_questions.size());
  text = (GuiText*)GetElementByName(m_gui, "instructions2-text");
  Assert(text);
  text->SetText(s);
}

void GSLikertQuestions::OnChoiceButton(int choice)
{
  std::cout << "Got choice: " << choice << "\n";
  // TODO Save and go to next Q. If no more Qs, return to menu state

  //Assert(m_testId != -1);
  TheCogTestResults::Instance()->StoreResult(
    new Result(m_testId, "q_" + ToString(m_currentQuestion), ToString(choice)));

  m_currentQuestion++;
  if (m_currentQuestion >= (int)m_questions.size())
  {
    // We are done here.
    Finished();
  }
  else
  {
    SetQuestion();
  }
}

void GSLikertQuestions::OnActive()
{
  GSCogTestBase::OnActive();

  m_gui = LoadGui(m_guiFilename);
  Assert(m_gui);
  LoadCommonGui();

  // Set up command handler for each button
  Assert(m_numButtons >= 0);
  for (int i = 0; i < m_numButtons; i++)
  {
    std::string buttonName = "button" + ToString(i + 1);
    GuiElement* elem = GetElementByName(m_gui, buttonName);
    Assert(elem);
    // Questions giev results 1..5 for 5-choice test, with 0 for decline to answer
    elem->SetCommand(new LikertButtonCommand(i + 1));
  }

  // Decline to answer button
  GuiElement* elem = GetElementByName(m_gui, "button-decline");
  Assert(elem);
  elem->SetCommand(new LikertButtonCommand(0));

  m_currentQuestion = 0;
  SetQuestion();
}

void GSLikertQuestions::Finished() 
{
  // ?
  TheGSCogTestMenu::Instance()->AdvanceToNextTest();
  TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
}

}
