#pragma once
#include "Plane.h"

namespace Amju
{
class AABB;

PlaneResult Intersects(const AABB& ab, const Plane& plane, float* penDepth);
}
