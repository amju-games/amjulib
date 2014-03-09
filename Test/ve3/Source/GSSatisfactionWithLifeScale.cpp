#include "GSSatisfactionWithLifeScale.h"
#include "CogTestResults.h"

namespace Amju
{
GSSatisfactionWithLifeScale::GSSatisfactionWithLifeScale()
{
  m_testId = AMJU_COG_TEST_SATISFACTIONWITHLIFE;
  m_guiFilename = "gui-swls.txt";

  // Satisfaction With Life Scale - 7 possible responses, plus option to decline.
  m_numButtons = 7;

  // The text for these 5 Qs is from Deiner et al. (1985).

  m_questions.push_back("\"In most ways my life is close to my ideal.\"");
  m_questions.push_back("\"The conditions of my life are excellent.\"");
  m_questions.push_back("\"I am satisfied with my life.\"");
  m_questions.push_back("\"So far I have gotten the important things I want in life.\"");
  m_questions.push_back("\"If I could live my life over, I would change almost nothing.\"");
}

}
