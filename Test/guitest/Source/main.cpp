#ifndef IPHONE

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
#include <CursorManager.h>
#include <Font.h>
#include <GuiElement.h>
#include "GSGuiTest.h"

namespace Amju
{
// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false, "GUI test!");

void ReportError(const std::string& str)
{
  std::cout << str << "\n";
}

void StartUp()
{
  TheCursorManager::Instance()->Load(Vec2f(0.025f, -0.08f));
  TheResourceManager::Instance()->AddLoader("font", FontLoader);

  TheGame::Instance()->SetCurrentState(TheGSGuiTest::Instance());

  GuiElement::SetGlobalScale(1.0f);
}
}

#endif // IPHONE

