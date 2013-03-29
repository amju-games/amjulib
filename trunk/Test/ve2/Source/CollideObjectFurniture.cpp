#include <AmjuFirst.h>
#include "Furniture.h"
#include <AmjuFinal.h>

namespace Amju
{
void CollideObjectFurniture(GameObject* go1, GameObject* go2)
{
  Assert(dynamic_cast<Ve1Object*>(go1));
  Assert(dynamic_cast<Ve1Object*>(go2));

  Ve1Object* go = (Ve1Object*)go1;
  Ve1Object* furn = (Ve1Object*)go2; // furniture - immovable

  /* 
  Move back so not interpenetrating:
  Boxes now intersect in all axes. Moving away by penetration depth in any
  axis will resolve the intersection. To choose which axis, use the previous
  position. E.g. if previously not intersecting in X, move away in X axis.
  */

  // Intersecton region
  AABB ir = furn->GetAABB()->Intersection(*(go->GetAABB()));
  Vec3f goPos = go->GetPos();
  const Vec3f& oldPos = go->GetOldPos();

  AABB oldBox = *(go->GetAABB());
  Vec3f move = oldPos - goPos;
  oldBox.Translate(move);
  // Oldbox should not be intersecting in one or more axes

  const AABB& fBox = *(furn->GetAABB());

  Vec3f penDist(
    ir.GetMax(0) - ir.GetMin(0), 
    ir.GetMax(1) - ir.GetMin(1),
    ir.GetMax(2) - ir.GetMin(2));
  penDist *= 1.01f; // move away a bit more to be sure of clearing the collision

  if (oldBox.GetMax(0) < fBox.GetMin(0))
  {
    // Old box to left of fence, so move away to the left
    goPos.x -= penDist.x; 
  }
  else if (oldBox.GetMin(0) > fBox.GetMax(0))
  {
    goPos.x += penDist.x;
  }
  else if (oldBox.GetMax(1) < fBox.GetMin(1))
  {
    goPos.y -= penDist.y;
  }
  else if (oldBox.GetMin(1) > fBox.GetMax(1))
  {
    goPos.y += penDist.y;
  }
  else if (oldBox.GetMax(2) < fBox.GetMin(2))
  {
    goPos.z -= penDist.z;
  }
  else if (oldBox.GetMin(2) > fBox.GetMax(2))
  {
    goPos.z += penDist.z;
  }

  go->SetPos(goPos);
}

}
