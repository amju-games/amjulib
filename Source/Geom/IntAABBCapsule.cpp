#include "IntAABBCapsule.h"
#include "AABB.h"
#include "Capsule.h"
#include "ClipLineSegBox.h"

namespace Amju
{
bool Intersects(const AABB& aabb, const Capsule& cap)
{
  float r = cap.m_radius;
  AABB ab(
    aabb.GetMin(0) - r, aabb.GetMax(0) + r,
    aabb.GetMin(1) - r, aabb.GetMax(1) + r,
    aabb.GetMin(2) - r, aabb.GetMax(2) + r);

  return Clip(cap.m_lineseg, ab, nullptr);
}
}

