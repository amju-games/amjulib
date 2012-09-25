#ifdef WIN32
#if defined(_DEBUG)
#pragma comment(lib, "../../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../../Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

#ifdef WIN32
// Windows: use OpenGL or Direct 3D - can choose at runtime, 
// so don't make this a compile time switch ?
// OR: allow compile-time switch to completely remove DX or OpenGL, 
// reducing DLL dependencies

// Define one of these - this chooses the API at compile time ATM
#define AMJU_USE_OPENGL
//#define AMJU_USE_DX9
//#define AMJU_USE_DX11

#endif // WIN32

#include <main.h>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include "GSViewObj.h"

namespace Amju
{
// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false, "View OBJ");

void StartUp()
{
  TheGame::Instance()->SetCurrentState(TheGSViewObj::Instance());
}
}

