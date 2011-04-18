#include "DrawOBB3.h"
#include <AmjuGL.h>

namespace Amju
{
void DrawOBB3(const OBB3& b)
{
  AmjuGL::PushAttrib(AmjuGL::AMJU_TEXTURE_2D | AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);

  Vec3f c[8];
  b.GetCorners(c);

  for (int i = 0; i < 4; i++)
  {
    int n = i < 3 ? i + 1 : 0; // next
    int i4 = i + 4;
    int n4 = n + 4;
    // Bottom
    AmjuGL::DrawLine(AmjuGL::Vec3(c[i].x, c[i].y, c[i].z), 
                     AmjuGL::Vec3(c[n].x, c[n].y, c[n].z));

    // Top
    AmjuGL::DrawLine(AmjuGL::Vec3(c[i4].x, c[i4].y, c[i4].z), 
                     AmjuGL::Vec3(c[n4].x, c[n4].y, c[n4].z));

    // Sides
    AmjuGL::DrawLine(AmjuGL::Vec3(c[i].x,  c[i].y,  c[i].z), 
                     AmjuGL::Vec3(c[i4].x, c[i4].y, c[i4].z));
  }
  AmjuGL::PopAttrib();
}

void DrawSolidOBB3(const OBB3& b)
{
  Vec3f c[8];
  b.GetCorners(c);

  AmjuGL::Tris tris;
  tris.reserve(12);
  AmjuGL::Tri t;

  // Bottom
  // TODO work out normal
  t.m_verts[0] = AmjuGL::Vert(c[0].x, c[0].y, c[0].z, 0, 0,  0, -1, 0);
  t.m_verts[1] = AmjuGL::Vert(c[2].x, c[2].y, c[2].z, 1, 1,  0, -1, 0);
  t.m_verts[2] = AmjuGL::Vert(c[1].x, c[1].y, c[1].z, 1, 0,  0, -1, 0);
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[0].x, c[0].y, c[0].z, 0, 0,  0, -1, 0);
  t.m_verts[1] = AmjuGL::Vert(c[3].x, c[3].y, c[3].z, 0, 1,  0, -1, 0);
  t.m_verts[2] = AmjuGL::Vert(c[2].x, c[2].y, c[2].z, 1, 1,  0, -1, 0);
  tris.push_back(t);

  // Top
  t.m_verts[0] = AmjuGL::Vert(c[4].x, c[4].y, c[4].z, 1, 0,  0, 1, 0);
  t.m_verts[1] = AmjuGL::Vert(c[6].x, c[6].y, c[6].z, 1, 1,  0, 1, 0);
  t.m_verts[2] = AmjuGL::Vert(c[5].x, c[5].y, c[5].z, 0, 0,  0, 1, 0);
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[4].x, c[4].y, c[4].z, 1, 1,  0, 1, 0);
  t.m_verts[1] = AmjuGL::Vert(c[7].x, c[7].y, c[7].z, 0, 1,  0, 1, 0);
  t.m_verts[2] = AmjuGL::Vert(c[6].x, c[6].y, c[6].z, 0, 0,  0, 1, 0);
  tris.push_back(t);

  // Side
  /*
  t.m_verts[0] = AmjuGL::Vert(xmax, ymin, zmin, 1, 0,  0, 0, -1);
  t.m_verts[1] = AmjuGL::Vert(xmin, ymin, zmin, 0, 0,  0, 0, -1);
  t.m_verts[2] = AmjuGL::Vert(xmax, ymax, zmin, 1, 1,  0, 0, -1);
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(xmax, ymax, zmin, 1, 1,  0, 0, -1);
  t.m_verts[1] = AmjuGL::Vert(xmin, ymin, zmin, 0, 0,  0, 0, -1);
  t.m_verts[2] = AmjuGL::Vert(xmin, ymax, zmin, 0, 1,  0, 0, -1);
  tris.push_back(t);

  // Side
  t.m_verts[0] = AmjuGL::Vert(xmin, ymin, zmax, 0, 0,  0, 0, 1);
  t.m_verts[1] = AmjuGL::Vert(xmax, ymin, zmax, 1, 0,  0, 0, 1);
  t.m_verts[2] = AmjuGL::Vert(xmax, ymax, zmax, 1, 1,  0, 0, 1);
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(xmin, ymin, zmax, 0, 0,  0, 0, 1);
  t.m_verts[1] = AmjuGL::Vert(xmax, ymax, zmax, 1, 1,  0, 0, 1);
  t.m_verts[2] = AmjuGL::Vert(xmin, ymax, zmax, 0, 1,  0, 0, 1);
  tris.push_back(t);

  // Side
  t.m_verts[0] = AmjuGL::Vert(xmax, ymin, zmax, 0, 1,  1, 0, 0);
  t.m_verts[1] = AmjuGL::Vert(xmax, ymin, zmin, 0, 0,  1, 0, 0);
  t.m_verts[2] = AmjuGL::Vert(xmax, ymax, zmax, 1, 1,  1, 0, 0);
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(xmax, ymax, zmax, 1, 1,  1, 0, 0);
  t.m_verts[1] = AmjuGL::Vert(xmax, ymin, zmin, 0, 0,  1, 0, 0);
  t.m_verts[2] = AmjuGL::Vert(xmax, ymax, zmin, 1, 0,  1, 0, 0);
  tris.push_back(t);

  // Side
  t.m_verts[0] = AmjuGL::Vert(xmin, ymin, zmin, 0, 0,  -1, 0, 0);
  t.m_verts[1] = AmjuGL::Vert(xmin, ymin, zmax, 0, 1,  -1, 0, 0);
  t.m_verts[2] = AmjuGL::Vert(xmin, ymax, zmax, 1, 1,  -1, 0, 0);
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(xmin, ymin, zmin, 0, 0,  -1, 0, 0);
  t.m_verts[1] = AmjuGL::Vert(xmin, ymax, zmax, 1, 1,  -1, 0, 0);
  t.m_verts[2] = AmjuGL::Vert(xmin, ymax, zmin, 1, 0,  -1, 0, 0);
  tris.push_back(t);
  */
  AmjuGL::DrawTriList(tris);
}
}
