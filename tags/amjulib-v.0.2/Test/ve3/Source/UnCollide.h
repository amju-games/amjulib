#ifndef UNCOLLIDE_H_INCLUDED
#define UNCOLLIDE_H_INCLUDED

#include <GameObject.h>

namespace Amju
{
// Move object away from a bounding box, so not colliding.
// Uses previous pos to decide which direction to go to uncollide.
void UnCollide(GameObject* go, const Vec3f& oldPos, const AABB& aabb);
}

#endif
