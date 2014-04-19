#ifndef IPHONE

#include <main.h>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include "GSLighting.h"

namespace Amju
{
// Create global variable window info 
AmjuGLWindowInfo w(640, 480, false);

void StartUp()
{
#ifdef GEKKO
  File::SetRoot("/apps/community_test/data/", "/");
#endif

  TheGame::Instance()->SetCurrentState(TheGSLighting::Instance());
}
}

#endif // IPHONE
