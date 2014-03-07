#include "GSLonelinessScale.h"

namespace Amju
{
GSLonelinessScale::GSLonelinessScale()
{
  m_guiFilename = "gui-loneliness.txt";

  m_numButtons = 5;

  // The text for these 6 Qs is from De Jong Geirveld & Van Tilberg (2006).

  m_questions.push_back("\"I experience a general sense of emptiness.\"");
  m_questions.push_back("\"I miss having people around.\"");
  m_questions.push_back("\"I often feel rejected.\"");
  m_questions.push_back("\"There are plenty of people I can rely on when I have problems.\"");
  m_questions.push_back("\"There are many people I can trust completely.\"");
  m_questions.push_back("\"There are enough people I feel close to.\"");
}
}
