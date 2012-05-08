#include "QuitHandler.h"
#include <Game.h>

namespace Amju
{
bool QuitHandler::OnQuitEvent()
{
  TheGame::Instance()->OnQuitEvent();
  return true;
}
}

