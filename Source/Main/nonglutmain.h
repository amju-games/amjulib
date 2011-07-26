#ifndef MACOSX

#include <AmjuFirst.h>
#include <iostream>

#ifdef GEKKO
#include "AmjuGL-GCube.h"
#include "EventPollerImplWii.h"
#include "SoundWii.h"
#include <File.h>
#endif // GEKKO

#ifdef WIN32
// Assume we always use OpenGL for now
#include <AmjuGL-OpenGL.h>
#include <EventPollerImplSDL.h>
//#include <AmjuGL-OpenGLES.h> // TODO TEMP TEST
#include <AmjuGL-DX9.h>
#include <EventPollerImplWin32.h>
#include <BassSoundPlayer.h>
#endif // WIN32

#ifdef MACOSX
#include <AmjuGL-OpenGL.h>
#endif // MACOSX

#include <Game.h>
#include "StartUp.h"
#include <SoundManager.h>
#include <AmjuGLWindowInfo.h>
#include <AmjuFinal.h>

using namespace Amju;

// Windows: use OpenGL or Direct 3D 
#define USE_OPENGL_NOT_D3D

int main(int argc, char **argv)
{
  std::cout << "Hello, in main!\n";

#ifdef GEKKO
	AmjuGL::SetImpl(new AmjuGLGCube(false /* console */));
  TheEventPoller::Instance()->SetImpl(new EventPollerImplWii);
  TheSoundManager::Instance()->SetImpl(new SoundWii);
  // TODO Any way we can get the directory where the executable lives ?
  File::SetRoot("/apps/amju_ww/data/", "/");
#endif // GEKKO

#ifdef WIN32
//	AmjuGL::SetImpl(new AmjuGLOpenGLES);
#ifdef USE_OPENGL_NOT_D3D
  std::cout << "Using OpenGL\n";
  AmjuGL::SetImpl(new AmjuGLOpenGL);
  TheEventPoller::Instance()->SetImpl(new EventPollerImplSDL);
#else // USE_OPENGL_NOT_D3D
  std::cout << "Using DX9\n";
  // Must use Win32 event poller with D3D
  AmjuGL::SetImpl(new AmjuGLDX9((WNDPROC)&EventPollerImplWin32::WndProc));
  TheEventPoller::Instance()->SetImpl(new EventPollerImplWin32);
#endif // USE_OPENGL_NOT_D3D

  TheSoundManager::Instance()->SetImpl(new BassSoundPlayer);
#endif // WIN32

  // Initialise window etc
  //Amju::AmjuGLWindowInfo w(480, 320, false);
  Amju::AmjuGLWindowInfo w(640, 480, false);
  Amju::AmjuGL::CreateWindow(&w);
  Amju::AmjuGL::Init();
//  Amju::AmjuGL::SetScreenRotation(10.0f);

  StartUp();

  TheGame::Instance()->Run();

  return 0;
}
 
#endif // not MACOSX 
