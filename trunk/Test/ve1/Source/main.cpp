#ifdef WIN32
#if defined(_DEBUG)
#pragma comment(lib, "../../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../../Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

#define AMJU_USE_OPENGL

#include <main.h>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include <ResourceManager.h>
#include <ObjMesh.h>
#include <Font.h>
#include "GSMain.h"
#include "GSLogin.h"
#include <CursorManager.h>

namespace Amju
{
void ReportError(const std::string& str)
{
  std::cout << "ERROR: " << str << "\n";
}

// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false);

void StartUp()
{
  TheResourceManager::Instance()->AddLoader("font", FontLoader);
  // Use Text obj loader for now
  TheResourceManager::Instance()->AddLoader("obj", TextObjLoader);

  TheCursorManager::Instance()->Load();

  //TheGame::Instance()->SetCurrentState(TheGSMain::Instance());
  TheGame::Instance()->SetCurrentState(TheGSLogin::Instance());
}
}

