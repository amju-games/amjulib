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
  Vec2f size(100, 100); // = GetSize();
  Vec2f pos(0, 0); // = GetCombinedPos();

  AmjuGL::Vert verts[4] = 
  {
    AmjuGL::Vert(pos.x + size.x, pos.y - size.y,  0,   1, 0,   0, 1, 0), // x, y, z, u, v, nx, ny, nz  
    AmjuGL::Vert(pos.x + size.x, pos.y,           0,   1, 1,   0, 1, 0),
    AmjuGL::Vert(pos.x,          pos.y,           0,   0, 1,   0, 1, 0),
    AmjuGL::Vert(pos.x,          pos.y - size.y,  0,   0, 0,   0, 1, 0)
  };

  m_tris.clear();
  m_tris.reserve(2);
  AmjuGL::Tri tri;
  tri.m_verts[0] = verts[0];
  tri.m_verts[1] = verts[1];
  tri.m_verts[2] = verts[2];
  m_tris.push_back(tri);

  tri.m_verts[0] = verts[0];
  tri.m_verts[1] = verts[2];
  tri.m_verts[2] = verts[3];
  m_tris.push_back(tri);
}

void RoomNode::Draw()
{
//  Assert(m_texture);
//  m_texture->UseThisTexture();

  //AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  //AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::PushMatrix();
  AmjuGL::RotateX(-90);

  for (Room::TileMap::iterator it = m_room->m_tilemap.begin(); it != m_room->m_tilemap.end(); ++it)
  {
    std::string t = it->first;

    // TODO Check if this is badly inefficient
    Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes(t);
    Assert(tex);
    tex->UseThisTexture();

    Room::PosVec& posvec = it->second;
    for (unsigned int i = 0; i < posvec.size(); i++)
    {
      Vec2f pos = posvec[i];
      AmjuGL::PushMatrix();
      AmjuGL::Translate(pos.x * 100.0f, pos.y * -100.0f, 0);
      AmjuGL::DrawTriList(m_tris);
      AmjuGL::PopMatrix();
    }
  }

  AmjuGL::PopMatrix();
}
}