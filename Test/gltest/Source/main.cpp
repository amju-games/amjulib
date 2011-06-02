#ifndef MACOSX

#include <AmjuFirst.h>
#include <iostream>

#ifdef GEKKO
#include <AmjuGL-GCube.h>
#include <EventPollerImplWii.h>
#include <SoundWii.h>
#endif // GEKKO

#ifdef WIN32
// We can use OpenGL or DX9
#include <AmjuGL-OpenGL.h>
#include <AmjuGL-DX9.h>
#include <EventPollerImplWin32.h>
#include <EventPollerImplSDL.h>
#include <SoundSDLMixer.h>
#endif // WIN32

#include "GSMain.h"
#include "GSObb2Test.h"
#include "GSBelt.h"

#include <Game.h>
#include <Font.h>
#include <AlphaBmpLoader.h>
#include <ResourceManager.h>
#include <SoundManager.h>
#include <AmjuGLWindowInfo.h>
#include <AmjuFinal.h>

#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "../../../../AmjuLib/Build/Debug/AmjuLibMsvc.lib")
#else // _DEBUG
#pragma comment(lib, "../../../../AmjuLib/Build/Release/AmjuLibMsvc.lib")
#endif // _DEBUG
#endif // WIN32

using namespace Amju;

#define USE_OPENGL

int main(int argc, char **argv)
{
#ifdef GEKKO
  AmjuGL::SetImpl(new AmjuGLGCube(false /* console */));
  TheEventPoller::Instance()->SetImpl(new EventPollerImplWii);
  TheSoundManager::Instance()->SetImpl(new SoundWii);
  // TODO Any way we can get the directory where the executable lives ?
  File::SetRoot("/apps/amju_ww/data/", "/");
#endif // GEKKO

#ifdef WIN32
//	AmjuGL::SetImpl(new AmjuGLOpenGLES);
#ifdef USE_OPENGL
  AmjuGL::SetImpl(new AmjuGLOpenGL);
  TheEventPoller::Instance()->SetImpl(new EventPollerImplSDL);
#else
  // If you use DX9, you must use the Win32 Event Poller
  AmjuGL::SetImpl(new AmjuGLDX9((WNDPROC)&EventPollerImplWin32::WndProc) );
  TheEventPoller::Instance()->SetImpl(new EventPollerImplWin32);
#endif

  TheSoundManager::Instance()->SetImpl(new SoundSDLMixer);
#endif // WIN32

  // Initialise window etc
  Amju::AmjuGLWindowInfo w(640, 480, false);
  Amju::AmjuGL::CreateWindow(&w);
  Amju::AmjuGL::Init();
	
  TheResourceManager::Instance()->AddLoader("bmpa", BmpALoader);
  TheResourceManager::Instance()->AddLoader("font", FontLoader);

  TheResourceManager::Instance()->LoadResourceGroup("2dtext-group");

  TheGame::Instance()->SetCurrentState(TheGSBelt::Instance()); 

  TheGame::Instance()->Run();

  return 0;
}
 
#endif
 
