#include <GameObjectFactory.h>
#include <StringUtils.h>
#include <LoadVec2.h>
#include "Room.h"
#include "RoomNode.h"
#include "Ve1SceneGraph.h"
#include "LocalPlayer.h"
#include "ObjectUpdater.h"

namespace Amju
{
static Room* s_room = 0;
Room* GetRoom()
{
  Assert(s_room);
  return s_room;
}

GameObject* CreateRoom()
{
  return new Room;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Room::TYPENAME, CreateRoom);

Room::Room()
{
  m_tilesize = Vec2f(100, 100);
}

void Room::Update()
{
  float east = m_tilesize.x * m_gridsize.x;
  float south = m_tilesize.y * m_gridsize.y;

  float smallDist = 20.0f; // TODO CONFIG

  bool changeLoc = false;
  int destLocation = 0;
  Vec3f destPos;

  // Check if local player has left room
  Player* p = GetLocalPlayer();
  if (p)
  {
    Vec3f pos = p->GetPos();
    if (pos.x < 0)
    {
      int w = GetDest(AMJU_ROOM_W);
      if (w)
      {
        // Go to West room
        changeLoc = true;
        destLocation = w;
        destPos = pos;
        destPos.x = east - smallDist; // TODO Assumes all rooms same size
      }
      else
      {
        // Stop player moving West
        pos.x = 0;
        p->SetPos(pos);
      }
    }
    else if (pos.x > east)
    {
      int e = GetDest(AMJU_ROOM_E); 
      if (e)
      {
        // Go to East room
        changeLoc = true;
        destLocation = e;
        destPos = pos;
        destPos.x = smallDist; 
      }
      else
      {
        pos.x = east;
        p->SetPos(pos);
      }
    }

    if (pos.z > south) // off bottom
    {
      int s = GetDest(AMJU_ROOM_S);
      if (s)
      {
        // Go to West room
        changeLoc = true;
        destLocation = s;
        destPos = pos;
        destPos.z = smallDist; // TODO Assumes all rooms same size
      }
      else
      {
        // Stop player moving West
        pos.z = south;
        p->SetPos(pos);
      }
    }
    else if (pos.z < 0) // up, off top of room
    {
      int n = GetDest(AMJU_ROOM_N); 
      if (n)
      {
        // Go to East room
        changeLoc = true;
        destLocation = n;
        destPos = pos;
        destPos.z = south - smallDist; 
      }
      else
      {
        pos.z = 0;
        p->SetPos(pos);
      }
    }

    if (changeLoc)
    {
      SetLocalPlayerLocation(destLocation); // LocalPlayer

      // Just do this, and wait for round trip from server. Ensures consistency..?
      TheObjectUpdater::Instance()->SendChangeLocationReq(p->GetId(), destPos, destLocation);

      // Set new position immediately
      p->SetPos(destPos);
      p->MoveTo(destPos); // stop moving ?
    }

  }
}

bool Room::Load(File* f)
{
  SetId(-1); // only one room exists at a time, with this fixed ID
  s_room = this;

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

  if (!LoadVec2(f, &m_gridsize))
  {
    f->ReportError("Expected grid size");
    return false;
  }

  for (int y = 0; y < m_gridsize.y; y++)
  {
    if (!f->GetDataLine(&s))
    {
      f->ReportError("Expected grid line " + ToString(y));
      return false;
    }
    Strings strs = Split(s, ' ');
    if (strs.size() != m_gridsize.x)
    {
      f->ReportError("Grid size does not match tile data.");
      return false;
    }
    for (int x = 0; x < m_gridsize.x; x++)
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
  // Load destination room numbers to NSEW
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected destination rooms");
    return false;
  }
  Strings dests = Split(s, ',');
  if (dests.size() != 4)
  {
    f->ReportError("Expected FOUR comma-sep destination rooms");
    return false;
  }
  bool nonz = false;
  for (int i = 0; i < 4; i++)
  {
    m_dest[i] = ToInt(dests[i]);
    if (m_dest[i])
    {
      nonz = true;
    }
  }
  if (!nonz)
  {
    f->ReportError("Room has no destinations from it");
    return false;
  }

  return true;
}

int Room::GetDest(RoomDir d) const
{
  return m_dest[(int)d];
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
