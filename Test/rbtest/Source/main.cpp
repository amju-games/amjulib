#include <main.h>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include "GSBelt.h"

// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false);

void StartUp()
{
  TheGame::Instance()->SetCurrentState(TheGSBelt::Instance());
}

