#include "AmjuFirst.h"
#include "DrawAABB.h"
#include "AABB.h"
#include "AmjuGL.h"
#include "AmjuFinal.h"

namespace Amju
{
void DrawSolidAABB(const AABB& a)
{
  AMJU_CALL_STACK;

  float xmin = a.GetMin(0);
  float ymin = a.GetMin(1);
  float zmin = a.GetMin(2);

  float xmax = a.GetMax(0);
  float ymax = a.GetMax(1);
  float zmax = a.GetMax(2);

  AmjuGL::Tris tris;
  tris.reserve(12);
  AmjuGL::Tri t;

  // Bottom
  t.m_verts[0] = AmjuGL::Vert(xmin, ymin, zmin, 0, 0,  0, -1, 0);
  t.m_verts[1] = AmjuGL::Vert(xmax, ymin, zmin, 1, 0,  0, -1, 0);
  t.m_verts[2] = AmjuGL::Vert(xmax, ymin, zmax, 1, 1,  0, -1, 0);
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(xmin, ymin, zmin, 0, 0,  0, -1, 0);
  t.m_verts[1] = AmjuGL::Vert(xmax, ymin, zmax, 1, 1,  0, -1, 0);
  t.m_verts[2] = AmjuGL::Vert(xmin, ymin, zmax, 0, 1,  0, -1, 0);
  tris.push_back(t);

  // Top
  t.m_verts[0] = AmjuGL::Vert(xmax, ymax, zmin, 1, 0,  0, 1, 0);
  t.m_verts[1] = AmjuGL::Vert(xmin, ymax, zmin, 0, 0,  0, 1, 0);
  t.m_verts[2] = AmjuGL::Vert(xmax, ymax, zmax, 1, 1,  0, 1, 0);
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(xmax, ymax, zmax, 1, 1,  0, 1, 0);
  t.m_verts[1] = AmjuGL::Vert(xmin, ymax, zmin, 0, 0,  0, 1, 0);
  t.m_verts[2] = AmjuGL::Vert(xmin, ymax, zmax, 0, 1,  0, 1, 0);
  tris.push_back(t);

  // Side
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

  AmjuGL::DrawTriList(tris);
}

void DrawAABB(const AABB& a)
{
  AMJU_CALL_STACK;

  float xmin = a.GetMin(0);
  float ymin = a.GetMin(1);
  float zmin = a.GetMin(2);

  float xmax = a.GetMax(0);
  float ymax = a.GetMax(1);
  float zmax = a.GetMax(2);

  // Bottom
  AmjuGL::DrawLine(AmjuGL::Vec3(xmin, ymin, zmin), 
                   AmjuGL::Vec3(xmax, ymin, zmin));

  AmjuGL::DrawLine(AmjuGL::Vec3(xmax, ymin, zmin), 
                   AmjuGL::Vec3(xmax, ymin, zmax));

  AmjuGL::DrawLine(AmjuGL::Vec3(xmax, ymin, zmax), 
                   AmjuGL::Vec3(xmin, ymin, zmax));

  AmjuGL::DrawLine(AmjuGL::Vec3(xmin, ymin, zmax), 
                   AmjuGL::Vec3(xmin, ymin, zmin));

  // Top
  AmjuGL::DrawLine(AmjuGL::Vec3(xmin, ymax, zmin), 
                   AmjuGL::Vec3(xmax, ymax, zmin));

  AmjuGL::DrawLine(AmjuGL::Vec3(xmax, ymax, zmin), 
                   AmjuGL::Vec3(xmax, ymax, zmax));

  AmjuGL::DrawLine(AmjuGL::Vec3(xmax, ymax, zmax), 
                   AmjuGL::Vec3(xmin, ymax, zmax));

  AmjuGL::DrawLine(AmjuGL::Vec3(xmin, ymax, zmax), 
                   AmjuGL::Vec3(xmin, ymax, zmin));

  // Sides
  AmjuGL::DrawLine(AmjuGL::Vec3(xmin, ymin, zmin), 
                   AmjuGL::Vec3(xmin, ymax, zmin));

  AmjuGL::DrawLine(AmjuGL::Vec3(xmax, ymin, zmin), 
                   AmjuGL::Vec3(xmax, ymax, zmin));

  AmjuGL::DrawLine(AmjuGL::Vec3(xmax, ymin, zmax), 
                   AmjuGL::Vec3(xmax, ymax, zmax));

  AmjuGL::DrawLine(AmjuGL::Vec3(xmin, ymin, zmax), 
                   AmjuGL::Vec3(xmin, ymax, zmax));
}
}
