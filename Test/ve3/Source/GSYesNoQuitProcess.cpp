#include <AmjuFirst.h>
#include "GSYesNoQuitProcess.h"
#include <AmjuFinal.h>

namespace Amju
{
GSYesNoQuitProcess::GSYesNoQuitProcess()
{
  m_question = "Do you really want to quit ?";
  m_yesText = "quit";
  m_noText = "cancel";
}

void GSYesNoQuitProcess::OnYes()
{
  exit(0);
}

}


