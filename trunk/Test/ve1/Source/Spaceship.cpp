#include <GameObjectFactory.h>
#include "Spaceship.h"

namespace Amju
{
const char* Spaceship::TYPENAME = "spaceship";

GameObject* CreateSpaceship()
{
  return new Spaceship;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Spaceship::TYPENAME, CreateSpaceship);

const char* Spaceship::GetTypeName() const
{
  return TYPENAME;
}

Spaceship::Spaceship()
{
}
} // namespace

