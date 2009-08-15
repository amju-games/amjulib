#include "Rect.h"

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
