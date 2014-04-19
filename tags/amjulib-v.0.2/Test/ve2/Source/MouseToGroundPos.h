#ifndef MOUSE_TO_GROUND_POS_H_INCLUDED
#define MOUSE_TO_GROUND_POS_H_INCLUDED

#include <Vec2.h>
#include <Vec3.h>

namespace Amju
{
class CollisionMesh;

bool MouseToGroundPos(const CollisionMesh* m, const Vec2f& mousePos, Vec3f* groundPos);

// Find intersection of ouse line where Y = 0. Can't fail - always returns a coord.
Vec3f MouseToXZ(const Vec2f& mouseScreen);
}

#endif

