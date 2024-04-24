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

  // Enlarge the box to contain (x, y) if necessary.
  void SetIf(float x, float y);

  // Enlarge this Rect to contain r if necessary
  void Union(const Rect& r);

  bool IsPointIn(const Vec2f& point) const;
  bool Intersects(const Rect& rect) const;
  Rect CalcIntersectRegion(const Rect& rect) const;

  void Translate(float x, float y);
  void Scale(float x, float y);

  Vec2f GetSize() const;

  float GetMin(int axis) const;
  float GetMax(int axis) const;

private:
  float m_xmin, m_xmax, m_ymin, m_ymax;
};

// Return rect with min values set to large positive, and max values set to
//  large negative values, so SetIf/Union will always succeed on first call.
Rect EmptyRect();
}

#endif

