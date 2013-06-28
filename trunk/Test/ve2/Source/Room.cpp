#include <GameObjectFactory.h>
#include <StringUtils.h>
#include <LoadVec2.h>
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

bool Room::Load(File* f)
{
  int numTextures = 0;
  if (!f->GetInteger(&numTextures))
  {
    f->ReportError("Expected num textures.");
    return false;
  }

  Strings texNames;
  texNames.reserve(numTextures);
  std::string s;

  for (int i = 0; i < numTextures; i++)
  {
    if (!f->GetDataLine(&s))
    {
      f->ReportError("Expected texture name");
      return false;
    }
    texNames.push_back(s);
  }

  Vec2i gridSize;
  if (!LoadVec2(f, &gridSize))
  {
    f->ReportError("Expected grid size");
    return false;
  }

  for (int y = 0; y < gridSize.y; y++)
  {
    if (!f->GetDataLine(&s))
    {
      f->ReportError("Expected grid line " + ToString(y));
      return false;
    }
    Strings strs = Split(s, ' ');
    if (strs.size() != gridSize.x)
    {
      f->ReportError("Grid size does not match tile data.");
      return false;
    }
    for (int x = 0; x < gridSize.x; x++)
    {
      int t = ToInt(strs[x]);
      if (t >= (int)texNames.size())
      {
        f->ReportError("Tile number too big: " + strs[x] + " in grid pos (" + ToString(x) + ", " + ToString(y) + ")");
        return false;
      }
      std::string tex = texNames[t];

      // Group all tiles with same texture
      PosVec& posvec = m_tilemap[tex];
      posvec.push_back(Vec2f((float)x, (float)y));
    }
  }

  return true;
}

const char* Room::TYPENAME = "room";

const char* Room::GetTypeName() const
{
  return TYPENAME;
}

void Room::OnLocationEntry()
{
  RoomNode* rn = new RoomNode(this);
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  root->AddChild(rn);
  rn->Build(); // TODO Use tile data we loaded to make tiled textured quads..?

  // Big AABB so not culled
  m_aabb.Set(-1000, 1000, -1000, 1000, -1000, 1000);
  *(rn->GetAABB()) = m_aabb;

  m_sceneNode = rn;

}

}
