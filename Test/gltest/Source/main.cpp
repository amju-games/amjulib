#include <main.h>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include <Font.h>
#include "GSLighting.h"

// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false);

void StartUp()
{
  TheGame::Instance()->SetCurrentState(TheGSLighting::Instance());
  TheResourceManager::Instance()->AddLoader("font", FontLoader);
}

