#include <AmjuFirst.h>
#include <GameObjectFactory.h>
#include "Building.h"
#include "SceneCollisionMesh.h"
#include "Player.h"
#include "SetObjMeshCommand.h"
#include <AmjuFinal.h>

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

}

