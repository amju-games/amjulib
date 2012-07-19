#include "AmjuFirst.h"
#include "DrawRect.h"
#include "Rect.h"
#include "AmjuGL.h"
#include "AmjuFinal.h"

namespace Amju
{
void DrawSolidRect(const Rect& r)
{
  AMJU_CALL_STACK;

  float xmin = r.GetMin(0);
  float ymin = r.GetMin(1);

  float xmax = r.GetMax(0);
  float ymax = r.GetMax(1);

  AmjuGL::Tris tris;
  tris.reserve(2);
  AmjuGL::Tri t;
  t.m_verts[0] = AmjuGL::Vert(xmin, ymin, 0, 0, 0,  0, -1, 0);
  t.m_verts[1] = AmjuGL::Vert(xmax, ymin, 0, 0, 0,  0, -1, 0);
  t.m_verts[2] = AmjuGL::Vert(xmax, ymax, 0, 0, 0,  0, -1, 0);
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(xmin, ymin, 0, 0, 0,  0, -1, 0);
  t.m_verts[1] = AmjuGL::Vert(xmax, ymax, 0, 0, 0,  0, -1, 0);
  t.m_verts[2] = AmjuGL::Vert(xmin, ymax, 0, 0, 0,  0, -1, 0);
  tris.push_back(t);

  AmjuGL::DrawTriList(tris);
}

void DrawRect(const Rect& r)
{
  AMJU_CALL_STACK;

  AmjuGL::DrawLine(AmjuGL::Vec3(r.GetMin(0), r.GetMin(1), 0), 
                   AmjuGL::Vec3(r.GetMax(0), r.GetMin(1), 0));

  AmjuGL::DrawLine(AmjuGL::Vec3(r.GetMax(0), r.GetMin(1), 0), 
                   AmjuGL::Vec3(r.GetMax(0), r.GetMax(1), 0));

  AmjuGL::DrawLine(AmjuGL::Vec3(r.GetMax(0), r.GetMax(1), 0), 
                   AmjuGL::Vec3(r.GetMin(0), r.GetMax(1), 0));

  AmjuGL::DrawLine(AmjuGL::Vec3(r.GetMin(0), r.GetMax(1), 0), 
                   AmjuGL::Vec3(r.GetMin(0), r.GetMin(1), 0));
}
}