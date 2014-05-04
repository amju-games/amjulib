#include <Timer.h>
#include "UnCollide.h"
#include "Player.h"

namespace Amju
{
void UnCollide(GameObject* go, const Vec3f& oldPos, const AABB& aabb)
{
  Player* player = dynamic_cast<Player*>(go);

  /* 
  Move back so not interpenetrating:
  Boxes now intersect in all axes. Moving away by penetration depth in any
  axis will resolve the intersection. To choose which axis, use the previous
  position. E.g. if previously not intersecting in X, move away in X axis.
  */

  // Push away more?
  const float EXTRA = 1.1f;
  //const float MOVE_AWAY_VEL = 200.0f;
  const float dt = TheTimer::Instance()->GetDt();

  // Intersecton region
  AABB ir = aabb.Intersection(go->GetAABB());
  Vec3f goPos = go->GetPos();
  Vec3f goVel = go->GetVel();
  Vec3f moveToPos = player->GetMoveToPos();

  AABB oldBox = go->GetAABB();
  Vec3f move = oldPos - goPos;
  oldBox.Translate(move);
  // Oldbox should NOT be intersecting in one or more axes

  Vec3f penDist(
    ir.GetMax(0) - ir.GetMin(0), 
    ir.GetMax(1) - ir.GetMin(1),
    ir.GetMax(2) - ir.GetMin(2));
  penDist *= 1.01f; // move away a bit more to be sure of clearing the collision

  if (oldBox.GetMax(0) < aabb.GetMin(0))
  {
    // Old box to left of box, so move away to the left
    goPos.x -= (penDist.x * EXTRA);
    moveToPos.x = goPos.x;
    //goVel.x  = -MOVE_AWAY_VEL;
  }
  else if (oldBox.GetMin(0) > aabb.GetMax(0))
  {
    goPos.x += penDist.x * EXTRA;
    moveToPos.x = goPos.x;
    //goVel.x  = MOVE_AWAY_VEL;
  }
  /*
  else if (oldBox.GetMax(1) < aabb.GetMin(1))
  {
    goPos.y -= penDist.y;
  }
  else if (oldBox.GetMin(1) > aabb.GetMax(1))
  {
    goPos.y += penDist.y;
  }
  */
  else if (oldBox.GetMax(2) < aabb.GetMin(2))
  {
    goPos.z -= (penDist.z * EXTRA);
    moveToPos.z = goPos.z;
//    goVel.z  = -MOVE_AWAY_VEL;
  }
  else if (oldBox.GetMin(2) > aabb.GetMax(2))
  {
    goPos.z += penDist.z * EXTRA;
    moveToPos.z = goPos.z;
//    goVel.z  = MOVE_AWAY_VEL;
  }

  go->SetPos(goPos);
  player->MoveTo(moveToPos);

  //  go->SetVel(goVel);
}
}
