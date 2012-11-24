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
#include <SoundManager.h>
#include <EventPoller.h>
#include <GuiButton.h>
#include "GSProxy.h"
#include "Ve1SceneGraph.h"
#include "SaveConfig.h"
#include "QuitHandler.h"
#include "ResizeHandler.h"
#include "JoystickToCursor.h" 
#include "ROConfig.h"

namespace Amju
{
void ReportError(const std::string& str)
{
  std::cout << "ERROR: " << str << "\n";
}

// Create global variable window info
Amju::AmjuGLWindowInfo w(640, 480, false, "Crash Site - Experimental multi-player online game");

void StartUp()
{
  LoadConfig();

  SoundManager* sm = TheSoundManager::Instance();
  sm->SetImpl(new BassSoundPlayer);

  // Too soon, assets not copied yet!!
  // Sound can drown out text-to-speech, so max vol is lower than 1.0
  //float maxVol = ROConfig()->GetFloat("max-vol");
  //sm->SetSongMaxVolume(maxVol);
  //sm->SetWavMaxVolume(maxVol);

  GuiElement::SetTextToSpeechEnabled(false);

  // Set screen size if saved last time
  ConfigFile* cf = TheGameConfigFile::Instance();
  static const char* SCREEN_X = "screen-x";
  static const char* SCREEN_Y = "screen-y";

  if (cf->Exists(SCREEN_X))
  {
    w.SetWidth(cf->GetInt(SCREEN_X));
  }
  if (cf->Exists(SCREEN_Y))
  {
    w.SetHeight(cf->GetInt(SCREEN_Y));
  }

  TheEventPoller::Instance()->AddListener(new QuitHandler);
  TheEventPoller::Instance()->AddListener(new ResizeHandler);

  // Joystick can be used to control mouse cursor
  TheEventPoller::Instance()->AddListener(TheJoystickToCursor(), -999);

  TheResourceManager::Instance()->AddLoader("font", FontLoader);
  // Use Text obj loader for .obj files, binary loader for .objb
  TheResourceManager::Instance()->AddLoader("obj", TextObjLoader);
  TheResourceManager::Instance()->AddLoader("objb", BinaryObjLoader);

  GuiButton::SetClickFilename("Sound/click.wav"); // doesn't try to load it

  // Don't try to load any assets yet! Wait until we have copied assets as required

  // Create empty root node
  //ClearVe1SceneGraph();

  // Start with state that copies assets to Save Dir
  TheGame::Instance()->SetCurrentState(TheGSCopyAssets::Instance());

  SetProxy();
}
}


