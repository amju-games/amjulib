#include <algorithm>
#include "IntAABBSphere.h"
#include "AABB.h"
#include "Sphere.h"

namespace Amju
{
bool Intersects(const AABB& ab, const Sphere& s)
{
  // Find point in aabb closest to sphere
  Vec3f p = s.GetCentre();
  const Vec3f& c = p;

  p.x = std::max(p.x, ab.GetMax(0));
  p.x = std::min(p.x, ab.GetMin(0));

  p.y = std::max(p.y, ab.GetMax(1));
  p.y = std::min(p.y, ab.GetMin(1));

  p.z = std::max(p.z, ab.GetMax(2));
  p.z = std::min(p.z, ab.GetMin(2));

  // Is point p further from sphere centre than radius
  float sqd = (c - p).SqLen();
  float r = s.GetRadius();
  return (sqd < r * r);
}
}

