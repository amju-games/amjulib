#include <main.h>
#include <Game.h>
#include "GSRigidBody.h"

namespace Amju
{
void StartUp()
{
std::cout << "Hello!\n";

  TheGame::Instance()->SetCurrentState(TheGSRigidBody::Instance());
}
}

