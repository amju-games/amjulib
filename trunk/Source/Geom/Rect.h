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
  Rect(const Vec2f& corner1, const Vec2f& corner2);

  void Set(float xmin, float xmax, float ymin, float ymax);

  bool IsPointIn(const Vec2f& point) const;
  bool Intersects(const Rect& rect) const;
  Rect CalcIntersectRegion(const Rect& rect) const;

  void Translate(float x, float y);

  float GetMin(int axis) const;
  float GetMax(int axis) const;

private:
  float m_xmin, m_xmax, m_ymin, m_ymax;
};
}

#endif

