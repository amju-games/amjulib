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
/****
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include <ResourceManager.h>
#include <ObjMesh.h>
#include <Font.h>
#include "GSCopyAssets.h"
#include <File.h>
#include <Directory.h>
#include <ConfigFile.h>
#include <BassSoundPlayer.h>
#include "Ve1SceneGraph.h"
#include "SaveConfig.h"
#include "QuitHandler.h"
#include "JoystickToCursor.h" // TODO TEMP TEST

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
  TheSoundManager::Instance()->SetImpl(new BassSoundPlayer);

  GuiElement::SetTextToSpeechEnabled(false);

  LoadConfig();

  TheEventPoller::Instance()->AddListener(new QuitHandler);
  // TODO TEMP TEST
  TheEventPoller::Instance()->AddListener(new JoystickToCursor, -999);

  TheResourceManager::Instance()->AddLoader("font", FontLoader);
  // Use Text obj loader for .obj files, binary loader for .objb 
  TheResourceManager::Instance()->AddLoader("obj", TextObjLoader);
  TheResourceManager::Instance()->AddLoader("objb", BinaryObjLoader);

  // Don't try to load any assets yet! Wait until we have copied assets as required

  // Create empty root node
  ClearVe1SceneGraph();

  // Start with state that copies assets to Save Dir
  TheGame::Instance()->SetCurrentState(TheGSCopyAssets::Instance());
}
}
***/
