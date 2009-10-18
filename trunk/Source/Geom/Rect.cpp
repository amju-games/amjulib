#include "Rect.h"
#include "AmjuAssert.h"

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

bool Rect::IsPointIn(const Vec2f& point)
{
  return (
    point.x >= m_xmin && 
    point.x <= m_xmax && 
    point.y >= m_ymin && 
    point.y <= m_ymax);
}

/*
bool Rect::Intersects(const Rect& rect)
{
}

Rect Rect::CalcIntersectRegion(const Rect& rect)
{
}
*/
}
