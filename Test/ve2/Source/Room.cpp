#include <GameObjectFactory.h>
#include "Room.h"
#include "RoomNode.h"
#include "Ve1SceneGraph.h"

namespace Amju
{
GameObject* CreateRoom()
{
  return new Room;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Room::TYPENAME, CreateRoom);

void Room::Update()
{
}

bool Room::Load(File*)
{
  return true;
}

const char* Room::TYPENAME = "room";

const char* Room::GetTypeName() const
{
  return TYPENAME;
}

void Room::OnLocationEntry()
{
  RoomNode* rn = new RoomNode;
    SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  root->AddChild(rn);

  // Big AABB so not culled
  *(tsn->GetAABB()) = m_aabb;

  m_sceneNode = rn;

}

}
