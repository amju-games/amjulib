#ifdef WIN32
// Windows: use OpenGL or Direct 3D - can choose at runtime, 
// so don't make this a compile time switch ?
// OR: allow compile-time switch to completely remove DX or OpenGL, 
// reducing DLL dependencies

// Define one of these - this chooses the API at compile time ATM
#define AMJU_USE_OPENGL
#define AMJU_USE_GLUT
//#define AMJU_USE_DX9
//#define AMJU_USE_DX11

#endif // WIN32

#ifdef MACOSX
#define AMJU_USE_OPENGL
#define AMJU_USE_GLUT
#endif

#include <main.h>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include <Font.h>
#include "GSLighting.h"
#include "GSTexture.h"
#include "GSShaderWave.h"
#include "GSShadow.h"
#include "GSEnvMap.h"

namespace Amju
{
// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false, "AmjuGL Test");

void StartUpBeforeCreateWindow()
{
}

void StartUpAfterCreateWindow()
{
  //TheGame::Instance()->SetCurrentState(TheGSTexture::Instance());
  //TheGame::Instance()->SetCurrentState(TheGSShaderWave::Instance());
  //TheGame::Instance()->SetCurrentState(TheGSShadow::Instance());
  TheGame::Instance()->SetCurrentState(TheGSEnvMap::Instance());

  TheResourceManager::Instance()->AddLoader("font", FontLoader);
}
}

