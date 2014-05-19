#ifndef AMJU_IOS

#define AMJU_USE_OPENGL
#define AMJU_USE_GLUT

#include <main.h>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include "GSSoftBody.h"

namespace Amju
{
// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false, "Soft body test");

void StartUpBeforeCreateWindow()
{
}

void StartUpAfterCreateWindow()
{
  TheGame::Instance()->SetCurrentState(TheGSSoftBody::Instance());
}
}

#endif // IPHONE

