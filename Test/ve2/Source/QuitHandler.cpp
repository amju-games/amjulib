#include <AmjuFirst.h>
#include "QuitHandler.h"
#include <Game.h>
#include <AmjuFinal.h>

namespace Amju
{
bool QuitHandler::OnQuitEvent()
{
  TheGame::Instance()->OnQuitEvent();
  return true;
}
}

