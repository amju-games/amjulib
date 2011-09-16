#include <main.h>
#include <Game.h>
#include "GSLighting.h"

namespace Amju
{
void StartUp()
{
  TheGame::Instance()->SetCurrentState(TheGSLighting::Instance());
}
}

