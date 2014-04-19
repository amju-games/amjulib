#include <Game.h>
#include "GSYesNoQuitSession.h"
#include "GSTitle.h"
#include "LogOut.h"

namespace Amju
{
GSYesNoQuitSession::GSYesNoQuitSession()
{
  m_question = "Do you really want to quit ?";
  m_yesText = "quit";
  m_noText = "cancel";
}

void GSYesNoQuitSession::OnYes()
{
  SendLogOut();
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance()); 
}

}


