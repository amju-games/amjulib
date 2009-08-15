/*
Amju Games source code (c) Copyright Jason Colman 2000-2009
*/

#include "AmjuFirst.h"
#include "AABB.h"
#include "AmjuGL.h"
#include "AmjuFinal.h"

namespace Amju
{
AABB::AABB() : m_xmin(0), m_xmax(0), m_ymin(0), m_ymax(0), m_zmin(0), m_zmax(0)
{
  AMJU_CALL_STACK;
}

AABB::AABB(
  float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) :
  m_xmin(xmin), m_xmax(xmax), 
  m_ymin(ymin), m_ymax(ymax),  
  m_zmin(zmin), m_zmax(zmax)
{
  AMJU_CALL_STACK;
}

float AABB::GetXSize() const
{
  return m_xmax - m_xmin;
}

float AABB::GetYSize() const
{
  return m_ymax - m_ymin;
}

float AABB::GetZSize() const
{
  return m_zmax - m_zmin;
}

void AABB::Draw() const
{
#ifdef _DEBUG
  AMJU_CALL_STACK;

  AmjuGL::DrawLine(AmjuGL::Vec3(m_xmin, m_ymin, m_zmin), 
                   AmjuGL::Vec3(m_xmax, m_ymin, m_zmin));

  AmjuGL::DrawLine(AmjuGL::Vec3(m_xmax, m_ymin, m_zmin), 
                   AmjuGL::Vec3(m_xmax, m_ymin, m_zmax));

  AmjuGL::DrawLine(AmjuGL::Vec3(m_xmax, m_ymin, m_zmax), 
                   AmjuGL::Vec3(m_xmin, m_ymin, m_zmax));

  AmjuGL::DrawLine(AmjuGL::Vec3(m_xmin, m_ymin, m_zmax), 
                   AmjuGL::Vec3(m_xmin, m_ymin, m_zmin));

  AmjuGL::DrawLine(AmjuGL::Vec3(m_xmin, m_ymax, m_zmin), 
                   AmjuGL::Vec3(m_xmax, m_ymax, m_zmin));

  AmjuGL::DrawLine(AmjuGL::Vec3(m_xmax, m_ymax, m_zmin), 
                   AmjuGL::Vec3(m_xmax, m_ymax, m_zmax));

  AmjuGL::DrawLine(AmjuGL::Vec3(m_xmax, m_ymax, m_zmax), 
                   AmjuGL::Vec3(m_xmin, m_ymax, m_zmax));

  AmjuGL::DrawLine(AmjuGL::Vec3(m_xmin, m_ymax, m_zmax), 
                   AmjuGL::Vec3(m_xmin, m_ymax, m_zmin));

  AmjuGL::DrawLine(AmjuGL::Vec3(m_xmin, m_ymin, m_zmin), 
                   AmjuGL::Vec3(m_xmin, m_ymax, m_zmin));

  AmjuGL::DrawLine(AmjuGL::Vec3(m_xmax, m_ymin, m_zmin), 
                   AmjuGL::Vec3(m_xmax, m_ymax, m_zmin));

  AmjuGL::DrawLine(AmjuGL::Vec3(m_xmax, m_ymin, m_zmax), 
                   AmjuGL::Vec3(m_xmax, m_ymax, m_zmax));

  AmjuGL::DrawLine(AmjuGL::Vec3(m_xmin, m_ymin, m_zmax), 
                   AmjuGL::Vec3(m_xmin, m_ymax, m_zmax));

#endif
}

void AABB::Set(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
  m_xmin = xmin;
  m_xmax = xmax;
  m_ymin = ymin;
  m_ymax = ymax;
  m_zmin = zmin;
  m_zmax = zmax;
}

void AABB::SetIf(float x, float y, float z)
{
  AMJU_CALL_STACK;

  if (x < m_xmin) m_xmin = x;
  if (x > m_xmax) m_xmax = x;

  if (y < m_ymin) m_ymin = y;
  if (y > m_ymax) m_ymax = y;

  if (z < m_zmin) m_zmin = z;
  if (z > m_zmax) m_zmax = z;}

bool AABB::Intersects(const AABB& b) const
{
  AMJU_CALL_STACK;

/*
  // These asserts aren't true when this box is constructed,
  // i.e. its volume is zero. 
  Assert(m_xmin <= m_xmax);
  Assert(m_ymin <= m_ymax);
  Assert(m_zmin <= m_zmax);
*/
  // Sometimes returns true when boxes touch but don't intersect.
  // So use SMALLEST offset to stop this happening..?
  static const float SMALLEST = 0;
  return (
    m_xmax > (b.m_xmin + SMALLEST) &&
    b.m_xmax > (m_xmin + SMALLEST) &&
    m_ymax > (b.m_ymin + SMALLEST) &&
    b.m_ymax > (m_ymin + SMALLEST) &&
    m_zmax > (b.m_zmin + SMALLEST) &&
    b.m_zmax > (m_zmin + SMALLEST) );
}

void AABB::Translate(const Vec3f& v)
{
  m_xmax += v.x;
  m_xmin += v.x;

  m_ymax += v.y;
  m_ymin += v.y;

  m_zmax += v.z;
  m_zmin += v.z;
}
}

