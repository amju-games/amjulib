#include <AmjuFirst.h>
#include "DrawOBB2.h"
#include <AmjuGL.h>
#include <AmjuFinal.h>

namespace Amju
{
void DrawSolidOBB2(const OBB2& b)
{
  Vec2f c[4];
  b.GetCorners(c);

  AmjuGL::Tris tris;
  tris.reserve(2);
  AmjuGL::Tri t;
  t.m_verts[0] = AmjuGL::Vert(c[0].x, c[0].y, 0, 0, 0,  0, -1, 0);
  t.m_verts[1] = AmjuGL::Vert(c[2].x, c[2].y, 0, 0, 0,  0, -1, 0);
  t.m_verts[2] = AmjuGL::Vert(c[1].x, c[1].y, 0, 0, 0,  0, -1, 0);
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[0].x, c[0].y, 0, 0, 0,  0, -1, 0);
  t.m_verts[1] = AmjuGL::Vert(c[3].x, c[3].y, 0, 0, 0,  0, -1, 0);
  t.m_verts[2] = AmjuGL::Vert(c[2].x, c[2].y, 0, 0, 0,  0, -1, 0);
  tris.push_back(t);

  AmjuGL::DrawTriList(tris);
}

void DrawOBB2(const OBB2& b)
{
  Vec2f c[4];
  b.GetCorners(c);

  AmjuGL::DrawLine(AmjuGL::Vec3(c[0].x, c[0].y, 0),
                   AmjuGL::Vec3(c[1].x, c[1].y, 0));

  AmjuGL::DrawLine(AmjuGL::Vec3(c[1].x, c[1].y, 0),
                   AmjuGL::Vec3(c[2].x, c[2].y, 0));

  AmjuGL::DrawLine(AmjuGL::Vec3(c[2].x, c[2].y, 0),
                   AmjuGL::Vec3(c[3].x, c[3].y, 0));

  AmjuGL::DrawLine(AmjuGL::Vec3(c[3].x, c[3].y, 0),
                   AmjuGL::Vec3(c[0].x, c[0].y, 0));
}
}
