#include <GameObjectFactory.h>
#include "Building.h"
#include "SceneCollisionMesh.h"
#include "Player.h"

namespace Amju
{
const char* Building::TYPENAME = "building";


GameObject* CreateBuilding()
{
  return new Building;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Building::TYPENAME, CreateBuilding);

const char* Building::GetTypeName() const
{
  return TYPENAME;
}

void Building::OnPlayerCollision(Player* player)
{
  // TODO per tri collision det/response
}

void Building::OnLocationEntry()
{
  // Create scene node, add to scene graph
}

void Building::OnLocationExit()
{
  // Remove node from graph
}

void Building::SetKeyVal(const std::string& key, const std::string& val)
{
  Ve1Object::SetKeyVal(key, val);

}
}

