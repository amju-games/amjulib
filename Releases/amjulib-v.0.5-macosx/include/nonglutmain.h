#include <AmjuFirst.h>
#include <iostream>

#ifdef GEKKO
#include "AmjuGL-GCube.h"
#include "EventPollerImplWii.h"
#include "SoundWii.h"
#include <File.h>
#include <Pause.h>
#endif // GEKKO

#ifdef WIN32
#include <AmjuGLWindowInfo.h>
#include <AmjuGL-Null.h>
#include <AmjuGL-OpenGL.h>
#include <AmjuGL-DX9.h>
#include <AmjuGL-DX11.h>
#include <EventPollerImplSDL.h>
#include <EventPollerImplWin32.h>
#include <BassSoundPlayer.h>
#endif // WIN32

#ifdef MACOSX
#include <AmjuGL-Null.h>
#include <EventPoller.h>
#endif // MACOSX

#ifdef IPHONE
#include <AmjuGL-Null.h>
#include <EventPoller.h>
#include <BassSoundPlayer.h>
#endif

#include <Game.h>
#include <SoundManager.h>
#include <AmjuFinal.h>

#ifdef CreateWindow
#undef CreateWindow
#endif

using namespace Amju;

namespace Amju
{
extern Amju::AmjuGLWindowInfo w;
bool CreateWindowSDLOpenGL(AmjuGLWindowInfo* w);
}

int main(int argc, char **argv)
{
  std::cout << "Hello, in main!\n";

#ifdef GEKKO
#ifdef AMJU_CONSOLE // define for console-only debugging
  AmjuGLImpl* impl = new AmjuGLGCubeConsole;
  impl->Init();
  AmjuGL::SetImpl(impl);
std::cout << "\n\n\n\n\nStarting...\n";

#else
  AmjuGL::SetImpl(new AmjuGLGCube);
#endif

  TheEventPoller::Instance()->SetImpl(new EventPollerImplWii);
  TheSoundManager::Instance()->SetImpl(new SoundWii);
#endif // GEKKO

#ifdef MACOSX
#ifdef AMJU_CONSOLE
  std::cout << "Mac console build\n";
  AmjuGL::SetImpl(new AmjuGLNull);
  TheEventPoller::Instance()->SetImpl(new EventPollerImplGeneric);
#else
#error Unexpectedly, for a Mac build I r in nonglutmain but AMJU_CONSOLE not defined
#endif // AMJU_CONSOLE
#endif // MACOSX

#ifdef WIN32

#ifdef AMJU_CONSOLE
  std::cout << "Windows console build\n";
  AmjuGL::SetImpl(new AmjuGLNull);
  TheEventPoller::Instance()->SetImpl(new EventPollerImplGeneric);
#else

#if !defined(AMJU_USE_OPENGL) && !defined(AMJU_USE_DX9) && !defined(AMJU_USE_DX11)
#error Dude, ATM you must define one of AMJU_USE_OPENGL, AMJU_USE_DX9 or AMJU_USE_DX11 in your main.cpp
#endif

#ifdef AMJU_USE_OPENGL
  std::cout << "Using OpenGL\n";
  AmjuGL::SetImpl(new AmjuGLOpenGL(CreateWindowSDLOpenGL));
  TheEventPoller::Instance()->SetImpl(new EventPollerImplSDL);
#endif // AMJU_USE_OPENGL

#ifdef AMJU_USE_DX9
  std::cout << "Using DX9\n";
  // Must use Win32 event poller with D3D
  AmjuGL::SetImpl(new AmjuGLDX9((WNDPROC)&EventPollerImplWin32::WndProc));
  TheEventPoller::Instance()->SetImpl(new EventPollerImplWin32);
#endif // AMJU_USE_DX9

#ifdef AMJU_USE_DX11
  std::cout << "Using DX11, woohoo\n";
  // Must use Win32 event poller with D3D
  AmjuGL::SetImpl(new AmjuGLDX11((WNDPROC)&EventPollerImplWin32::WndProc));
  TheEventPoller::Instance()->SetImpl(new EventPollerImplWin32);
#endif // AMJU_USE_DX11

  //TheSoundManager::Instance()->SetImpl(new BassSoundPlayer);
#endif // AMJU_CONSOLE
#endif // WIN32

#ifdef IPHONE
#ifdef AMJU_CONSOLE
  std::cout << "iPhone console build\n";
  AmjuGL::SetImpl(new AmjuGLNull);
  TheEventPoller::Instance()->SetImpl(new EventPollerImplGeneric);
  TheSoundManager::Instance()->SetImpl(new BassSoundPlayer);
#else
#error Non-console iphone build should be using the objective-C framework
#endif // AMJU_CONSOLE
#endif // IPHONE

#ifdef AMJU_CONSOLE
  std::cout << "Just before startup...\n";
#endif

  StartUpBeforeCreateWindow();

  Amju::AmjuGL::CreateWindow(&w);  // Before Init() for all impls

#ifdef AMJU_CONSOLE
  std::cout << "Just before Init...\n";
#endif

  Amju::AmjuGL::Init();

#ifdef AMJU_CONSOLE
  std::cout << "Just before startup2...\n";
#endif

  StartUpAfterCreateWindow();

#ifdef AMJU_CONSOLE
  std::cout << "Just before run...\n";
#endif

  TheGame::Instance()->Run();

  return 0;
}
 
