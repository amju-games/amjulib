#include <AmjuAssert.h>
#include <Vec2.h>
#include "RoomNode.h"
#include "Room.h"

namespace Amju
{
RoomNode::RoomNode(Room* room) : m_room(room)
{
}

void RoomNode::Build()
{
  Vec2f size = m_room->m_tilesize;
  Vec2f pos(0, 0); // = GetCombinedPos();

  AmjuGL::Vert verts[4] = 
  {
    AmjuGL::Vert(pos.x + size.x, pos.y - size.y,  0,   1, 0,   0, 1, 0), // x, y, z, u, v, nx, ny, nz  
    AmjuGL::Vert(pos.x + size.x, pos.y,           0,   1, 1,   0, 1, 0),
    AmjuGL::Vert(pos.x,          pos.y,           0,   0, 1,   0, 1, 0),
    AmjuGL::Vert(pos.x,          pos.y - size.y,  0,   0, 0,   0, 1, 0)
  };

  AmjuGL::Tris tris;
  tris.clear();
  tris.reserve(2);
  AmjuGL::Tri tri;
  tri.m_verts[0] = verts[0];
  tri.m_verts[1] = verts[1];
  tri.m_verts[2] = verts[2];
  tris.push_back(tri);

  tri.m_verts[0] = verts[0];
  tri.m_verts[1] = verts[2];
  tri.m_verts[2] = verts[3];
  tris.push_back(tri);

  m_triList = MakeTriList(tris);
}

void RoomNode::Draw()
{
  //AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::PushAttrib(AmjuGL::AMJU_DEPTH_WRITE | AmjuGL::AMJU_BLEND);

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::PushMatrix();
  AmjuGL::RotateX(-90);

  Vec2f size = m_room->m_tilesize;

  for (int i = 0; i < Room::NUM_TILE_MAPS; i++) // each tile map
  {
    if (i > 0)
    {
      AmjuGL::Enable(AmjuGL::AMJU_BLEND);
    }

    for (Room::TileMap::iterator it = m_room->m_tilemap[i].begin(); it != m_room->m_tilemap[i].end(); ++it)
    {
      std::string t = it->first;

      // TODO Check if this is badly inefficient
      Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes(t);
      Assert(tex);
      tex->UseThisTexture();

      Room::TileVec& tilevec = it->second;
      for (unsigned int i = 0; i < tilevec.size(); i++)
      {
        Vec2f& pos = tilevec[i].m_pos;
        AmjuGL::PushMatrix();
        AmjuGL::Translate(pos.x * size.x, pos.y * -size.y, 0);

        Assert(m_triList);
        AmjuGL::Draw(m_triList);

        AmjuGL::PopMatrix();
      }
    }
  }

  AmjuGL::PopMatrix();
  AmjuGL::PopAttrib();
}
}
