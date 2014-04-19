#pragma once

namespace Amju
{
class AABB;
class Sphere;

bool Intersects(const AABB& aabb, const Sphere& s);
}

