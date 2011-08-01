#include <main.h>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
//#include "GSEnterName.h"
#include "GSLighting.h"

// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false);

namespace Amju
{
void StartUp()
{
#ifdef GEKKO
  File::SetRoot("/apps/community_test/data/", "/");
#endif

  TheGame::Instance()->SetCurrentState(TheGSLighting::Instance());
}
}

