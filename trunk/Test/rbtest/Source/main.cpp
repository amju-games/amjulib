#ifdef WIN32
#if defined(_DEBUG)
#pragma comment(lib, "../../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../../Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

#define AMJU_USE_OPENGL

#include <main.h>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include "GSBelt.h"
#include "GSRigidBody.h"
#include "GSSpringMesh.h"
#include <OBB3.h>

namespace Amju
{
// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false);

void StartUp()
{
  TheGame::Instance()->SetCurrentState(
//    TheGSSpringMesh::Instance()
    TheGSRigidBody::Instance()
//    TheGSBelt::Instance()
  );
}
}

