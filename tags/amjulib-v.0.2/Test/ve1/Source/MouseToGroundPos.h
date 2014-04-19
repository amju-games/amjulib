#ifndef MOUSE_TO_GROUND_POS_H_INCLUDED
#define MOUSE_TO_GROUND_POS_H_INCLUDED

#include <Vec2.h>
#include <Vec3.h>

namespace Amju
{
class CollisionMesh;

bool MouseToGroundPos(const CollisionMesh* m, const Vec2f& mousePos, Vec3f* groundPos);
}

#endif

