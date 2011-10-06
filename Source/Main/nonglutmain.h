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
#include <AmjuGL-OpenGL.h>
#include <EventPollerImplSDL.h>
#include <AmjuGL-DX9.h>
#include <AmjuGL-DX11.h>
#include <EventPollerImplWin32.h>
#include <BassSoundPlayer.h>
#endif // WIN32

#ifdef MACOSX
#include <AmjuGL-OpenGL.h>
#endif // MACOSX

#ifdef IPHONE
#include <AmjuGL-Null.h>
#include <EventPollerImplGeneric.h>
#include <BassSoundPlayer.h>
#endif

#include <Game.h>
#include <SoundManager.h>
#include <AmjuGLWindowInfo.h>
#include <AmjuFinal.h>

using namespace Amju;

namespace Amju
{
extern Amju::AmjuGLWindowInfo w;
}

int main(int argc, char **argv)
{
  std::cout << "Hello, in main!\n";

#ifdef GEKKO
  AmjuGL::SetImpl(new AmjuGLGCube);
  TheEventPoller::Instance()->SetImpl(new EventPollerImplWii);
  TheSoundManager::Instance()->SetImpl(new SoundWii);
#endif // GEKKO

#ifdef WIN32

#if !defined(AMJU_USE_OPENGL) && !defined(AMJU_USE_DX9) && !defined(AMJU_USE_DX11)
#error Dude, ATM you must define one of AMJU_USE_OPENGL, AMJU_USE_DX9 or AMJU_USE_DX11 in your main.cpp
#endif

#ifdef AMJU_USE_OPENGL
  std::cout << "Using OpenGL\n";
  AmjuGL::SetImpl(new AmjuGLOpenGL);
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

  TheSoundManager::Instance()->SetImpl(new BassSoundPlayer);
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

  // Initialise window etc
std::cout << "Just before init...\n";

  Amju::AmjuGL::Init();

std::cout << "Just before create window...\n";

  Amju::AmjuGL::CreateWindow(&w); // TODO Check ret val
//  Amju::AmjuGL::SetScreenRotation(10.0f);

std::cout << "Just before startup...\n";
  StartUp();

std::cout << "Just before run...\n";
  TheGame::Instance()->Run();

  return 0;
}
 
#endif // not MACOSX 
