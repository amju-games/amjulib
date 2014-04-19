#pragma once

#include <StringUtils.h>
#include "GSCogTestBase.h"

namespace Amju
{
class GSLikertQuestions : public GSCogTestBase
{
public:
  GSLikertQuestions() : m_numButtons(-1), m_currentQuestion(0) {}

  virtual void OnActive();

  virtual void Finished() override; 

//  virtual void Update();
//  virtual void Draw();
//  virtual void Draw2d();

  void OnChoiceButton(int choice);

protected:
  void SetQuestion();

protected:
  // num buttons?
  int m_numButtons;

  // gui filename
  std::string m_guiFilename;

  Strings m_questions;
  int m_currentQuestion;
};
}
