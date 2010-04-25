#include "AmjuFirst.h"
#include "Rect.h"
#include <algorithm>
#include "AmjuAssert.h"
#include "AmjuFinal.h"

namespace Amju
{
Rect::Rect() : m_xmin(0), m_xmax(0), m_ymin(0), m_ymax(0)
{
}

Rect::Rect(float xmin, float xmax, float ymin, float ymax) :
  m_xmin(xmin), m_xmax(xmax), m_ymin(ymin), m_ymax(ymax)
{
}

void Rect::Set(float xmin, float xmax, float ymin, float ymax)
{
  m_xmin = xmin;
  m_xmax = xmax;
  m_ymin = ymin;
  m_ymax = ymax;
}

void Rect::Translate(float x, float y)
{
  m_xmin += x;
  m_xmax += x;
  m_ymin += y;
  m_ymax += y;
}

float Rect::GetMin(int axis) const
{
  Assert(axis < 2);
  const float* const f = &m_xmin;
  return f[axis * 2];
}

float Rect::GetMax(int axis) const
{
  Assert(axis < 2);
  const float* const f = &m_xmin;
  return f[axis * 2 + 1];
}

bool Rect::IsPointIn(const Vec2f& point) const
{
  return (
    point.x >= m_xmin && 
    point.x <= m_xmax && 
    point.y >= m_ymin && 
    point.y <= m_ymax);
}

/*
bool Rect::Intersects(const Rect& rect) const
{
}
*/

Rect Rect::CalcIntersectRegion(const Rect& r) const
{
  return Rect(
    std::max(m_xmin, r.m_xmin),
    std::min(m_xmax, r.m_xmax),
    std::max(m_ymin, r.m_ymin),
    std::min(m_ymax, r.m_ymax));
}
}
