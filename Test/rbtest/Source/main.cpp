#include <main.h>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include "GSBelt.h"
#include "GSRigidBody.h"
#include "GSSpringMesh.h"

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

