/*
Amju Games source code (c) Copyright Jason Colman 2000-2009
*/

#include "AmjuFirst.h"
#ifdef _DEBUG
#include <iostream>
#endif
#include "AABB.h"
#include "Matrix.h"
#include "AmjuAssert.h"
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

float AABB::GetMin(int axis) const
{
  Assert(axis < 3);
  const float* const f = &m_xmin;
  return f[axis * 2];
}

float AABB::GetMax(int axis) const
{
  Assert(axis < 3);
  const float* const f = &m_xmin;
  return f[axis * 2 + 1];
}

void AABB::SetMin(int axis, float min)
{
  float* const f = &m_xmin;
  f[axis * 2] = min;
}

void AABB::SetMax(int axis, float max)
{
  float* const f = &m_xmin;
  f[axis * 2 + 1] = max;
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
  if (z > m_zmax) m_zmax = z;
}

void AABB::Union(const AABB& b)
{
  AMJU_CALL_STACK;

  if (b.m_xmin < m_xmin) m_xmin = b.m_xmin;
  if (b.m_xmax > m_xmax) m_xmax = b.m_xmax;

  if (b.m_ymin < m_ymin) m_ymin = b.m_ymin;
  if (b.m_ymax > m_ymax) m_ymax = b.m_ymax;

  if (b.m_zmin < m_zmin) m_zmin = b.m_zmin;
  if (b.m_zmax > m_zmax) m_zmax = b.m_zmax;
}

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

void AABB::Transform(const Matrix& m)
{
  Vec3f min(m_xmin, m_ymin, m_zmin);
  Vec3f max(m_xmax, m_ymax, m_zmax);
  Vec3f newMin = min * m;
  Vec3f newMax = max * m;
  m_xmin = std::min(newMin.x, newMax.x);
  m_ymin = std::min(newMin.y, newMax.y);
  m_zmin = std::min(newMin.z, newMax.z);
  m_xmax = std::max(newMin.x, newMax.x);
  m_ymax = std::max(newMin.y, newMax.y);
  m_zmax = std::max(newMin.z, newMax.z);
}

AABB AABB::Intersection(const AABB& r) const
{
  return AABB(
    std::max(m_xmin, r.m_xmin),
    std::min(m_xmax, r.m_xmax),
    std::max(m_ymin, r.m_ymin),
    std::min(m_ymax, r.m_ymax),
    std::max(m_zmin, r.m_zmin),
    std::min(m_zmax, r.m_zmax));
}
}

