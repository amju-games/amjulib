#ifdef WIN32
#if defined(_DEBUG)
#pragma comment(lib, "../../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../../Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

#define AMJU_USE_OPENGL
#define AMJU_USE_GLUT
//#define AMJU_USE_DX9

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
#include <SAP.h>
#include "AvatarManager.h"
#include "Ve1SceneGraph.h"
#include "SaveConfig.h"
#include "SAPCollideFunc.h"
#include "QuitHandler.h"
#include "JoystickToCursor.h" // TODO TEMP TEST

namespace Amju
{
static const char* APP_NAME = "ve1"; // default name for App data dir

static const char* APP_NAME_CONFIG_KEY = "appname";

void ReportError(const std::string& str)
{
  std::cout << "ERROR: " << str << "\n";
}

// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false);

void StartUp()
{
  LoadConfig();

  std::string appname = APP_NAME;
  if (TheGameConfigFile::Instance()->Exists(APP_NAME_CONFIG_KEY))
  {
    appname = TheGameConfigFile::Instance()->GetValue(APP_NAME_CONFIG_KEY);
  }
  // TODO Get SaveDir from config or workding dir of executable
  File::SetRoot(GetSaveDir(appname) + "/Data/", "/");
  
  // Set SAP collide func
  //TheSAP::Instance()->SetCollideFunc(SAPCollideFunc);
  TheEventPoller::Instance()->AddListener(new QuitHandler);
  // TODO TEMP TEST
  TheEventPoller::Instance()->AddListener(new JoystickToCursor, -999);

  TheResourceManager::Instance()->AddLoader("font", FontLoader);
  // Use Text obj loader for .obj files, binary loader for .objb 
  TheResourceManager::Instance()->AddLoader("obj", TextObjLoader);
  TheResourceManager::Instance()->AddLoader("objb", BinaryObjLoader);

  TheCursorManager::Instance()->Load(Vec2f(0.025f, -0.08f));
  TheAvatarManager::Instance()->Load();

  // Create empty root node
  ClearVe1SceneGraph();

  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
}
}

