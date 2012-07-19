#ifndef OBB2_H
#define OBB2_H

#include <Vec2.h>

namespace Amju
{
// 2D oriented bounding box
class OBB2
{
public:
  OBB2();
  OBB2(const Vec2f& centre, const Vec2f& xAxis, const Vec2f& extents);
  void SetCentre(const Vec2f& centre);
  void SetXAxis(const Vec2f& axis);
  void SetExtents(const Vec2f& extents);
  void SetRotation(float rads);

  const Vec2f& GetCentre() const;
  const Vec2f& GetAxis(int n) const;
  const Vec2f& GetExtents() const;

  void GetCorners(Vec2f[4]) const;

  bool Intersects(const Vec2f& p, Vec2f* contactNormal, float* penetrationDepth) const;

private:
  Vec2f m_centre;
  Vec2f m_axes[2];
  Vec2f m_extents;
};

// Intersection test - SAT
bool Intersects(const OBB2& b1, const OBB2& b2);
}

#endif
