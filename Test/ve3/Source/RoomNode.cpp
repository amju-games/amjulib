#include <AmjuAssert.h>
#include <Vec2.h>
#include "RoomNode.h"
#include "Room.h"

namespace Amju
{
RoomNode::RoomNode(const Tile& tile) : m_tile(tile)
{
  if (m_tile.m_blend)
  {
    SetBlended(true);
  }
////  SetIsZReadEnabled(false);
}

void RoomNode::Build()
{
  Vec2f size = m_tile.m_size; 
  Vec3f pos(0, 0, 0); //// = m_tile.m_pos; 

  Matrix mat;
  mat.Translate(Vec3f(m_tile.m_pos.x, m_tile.m_pos.y, m_tile.m_pos.y));
  SetLocalTransform(mat);
  CombineTransform();

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
  AmjuGL::PushAttrib(AmjuGL::AMJU_DEPTH_WRITE | AmjuGL::AMJU_BLEND);

  if (m_tile.m_blend)
  {
    AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  }

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);
  AmjuGL::RotateX(-90);

  Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes(m_tile.m_tex);
  Assert(tex);
  tex->UseThisTexture();

  Vec2f size = m_tile.m_size;
  Vec3f pos = m_tile.m_pos;
  Assert(m_triList);
  AmjuGL::Draw(m_triList);
  AmjuGL::PopMatrix();
  AmjuGL::PopAttrib();
}
}
