#pragma once

namespace Amju
{
class AABB;
struct Capsule;

bool Intersects(const AABB& aabb, const Capsule& cap);
}

