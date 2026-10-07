// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#pragma once
#include "Plane.h"

namespace Amju
{
class AABB;

PlaneResult Intersects(const AABB& ab, const Plane& plane, float* penDepth);
}
