#ifndef AMJU_IOS

#ifdef WIN32
#if defined(_DEBUG)
#pragma comment(lib, "../../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../../Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

#define AMJU_USE_OPENGL
#define AMJU_USE_GLUT

#include <iostream>
#include <main.h>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include "GSSimplest.h"

namespace Amju
{
// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false, "Lua Example");

void ReportError(const std::string& str)
{
  std::cout << str << "\n";
}

void StartUpBeforeCreateWindow()
{
}

void StartUpAfterCreateWindow()
{
  TheGame::Instance()->SetCurrentState(TheGSLuaExample::Instance());
}
}

#endif // IPHONE

