#ifndef AMJU_RECT_H
#define AMJU_RECT_H

#include "Vec2.h"

namespace Amju
{
class Rect
{
public:
  Rect();
  Rect(float xmin, float xmax, float ymin, float ymax);
  void Set(float xmin, float xmax, float ymin, float ymax);

  bool IsPointIn(const Vec2f& point);
  bool Intersects(const Rect& rect);
  Rect CalcIntersectRegion(const Rect& rect);

private:
  float m_xmin, m_xmax, m_ymin, m_ymax;
};
}

#endif

