#ifndef LINE2_H
#define LINE2_H

#include "Vec2.h"

namespace Amju
{
template <class T>
struct Line2
{
  Line2(const Vec2<T>& p1, const Vec2<T>& p2) : pt1(p1), pt2(p2) {}
  Vec2<T> pt1, pt2;

  // Equality: two lines are equal if they share the same points. 
  bool operator==(const Line2& rhs) const;

  float Length() const;
};

// Returns true if the two given lines intersect. In this case *pResult is set
// to the intersection point.
template <typename T>
bool Intersects(const Line2<T>& line1, const Line2<T>& line2, Vec2<T>* pResult)
{
  const float Ax = line1.pt1.x;
  const float Ay = line1.pt1.y;
  const float Bx = line1.pt2.x;
  const float By = line1.pt2.y;
  const float Cx = line2.pt1.x;
  const float Cy = line2.pt1.y;
  const float Dx = line2.pt2.x;
  const float Dy = line2.pt2.y;

  // Denominator
  float d = (Bx - Ax) * (Dy - Cy) - (By - Ay) * (Dx - Cx);

  if (fabs(d) < SMALLEST)
  {
    // Parallel
    return false;
  }

  // Get unknowns
  float ua = ((Ay - Cy) * (Dx - Cx) - (Ax - Cx) * (Dy - Cy) ) / d;
  float ub = ((Ay - Cy) * (Bx - Ax) - (Ax - Cx) * (By - Ay) ) / d;

  if (ua < 0 || ua > 1 || ub < 0 || ub > 1)
  {
    return false;
  }

  pResult->x = Ax + ua * (Bx - Ax);
  pResult->y = Ay + ua * (By - Ay);

  return true;
}

}

#endif
