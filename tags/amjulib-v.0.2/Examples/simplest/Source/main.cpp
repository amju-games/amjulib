#ifndef AMJU_IOS

#ifdef WIN32
#if defined(_DEBUG)
#pragma comment(lib, "../../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../../Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

#if !defined(ANDROID_NDK)
#include <iostream>
#define AMJU_USE_OPENGL
#define AMJU_USE_GLUT
#include <main.h>
#endif

#ifdef ANDROID_NDK
#include <unistd.h>
#include <AmjuGL-OpenGLES.1.h>
#endif

#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include <EventPoller.h>
#include "GSSimplest.h"

namespace Amju
{
// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false, "Simplest");

void ReportError(const std::string& str)
{
#if !defined(ANDROID_NDK)
  std::cout << str << "\n";
#endif
}

void StartUpBeforeCreateWindow()
{
// Android: wait for debugger, or any crash at startup will be too soon 
// Thanks: http://stackoverflow.com/questions/17705109/how-to-debug-c-c-code-ndk-in-eclipse
#if defined(ANDROID_NDK) && !defined(NDEBUG)
usleep(5000 * 1000);
#endif

#ifdef ANDROID_NDK
  TheEventPoller::Instance()->SetImpl(new EventPollerImpl);
#endif
}

void StartUpAfterCreateWindow()
{
#ifdef ANDROID_NDK
  AmjuGL::SetImpl(new AmjuGLOpenGLES1);
#endif

  TheGame::Instance()->SetCurrentState(TheGSSimplest::Instance());
}
}

#endif // IPHONE

