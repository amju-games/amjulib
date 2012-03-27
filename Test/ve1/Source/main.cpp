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
#include "GSTitle.h"
#include "GSMain.h"
#include "GSLogin.h"
#include "GSAvatarMod.h"
#include <CursorManager.h>
#include <File.h>
#include <Directory.h>
#include <ConfigFile.h>
#include "AvatarManager.h"
#include "Ve1SceneGraph.h"

namespace Amju
{
static const char* APP_NAME = "ve1";

void ReportError(const std::string& str)
{
  std::cout << "ERROR: " << str << "\n";
}

// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false);

void StartUp()
{
  File::SetRoot(GetSaveDir(APP_NAME) + "/Data/", "/");
  
  TheGameConfigFile::Instance()->Load("game.cfg");

  // TODO Need to set SAP collide func

  TheResourceManager::Instance()->AddLoader("font", FontLoader);
  // Use Text obj loader for .obj files, binary loader for .objb 
  TheResourceManager::Instance()->AddLoader("obj", TextObjLoader);
  TheResourceManager::Instance()->AddLoader("objb", BinaryObjLoader);

  TheCursorManager::Instance()->Load(Vec2f(0.025f, -0.08f));
  TheAvatarManager::Instance()->Load();

  // Create empty root node
  ClearVe1SceneGraph();

  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
  //TheGame::Instance()->SetCurrentState(TheGSLogin::Instance());
}
}

