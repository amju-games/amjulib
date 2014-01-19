#include <AmjuAssert.h>
#include "IntAABBPlane.h"
#include "AABB.h"

namespace Amju
{
PlaneResult Intersects(const AABB& aabb, const Plane& plane, float* penDepth)
{
  // Get 'most positive' and 'most negative' box corners wrt plane normal.
  // Get distance for pos and neg corners.
  // This follows the same idea as Frustum culling, see 
  // http://www.lighthouse3d.com/opengl/viewfrustum/index.php?gatest3

  Vec3f pos(aabb.GetMin(0), aabb.GetMin(1), aabb.GetMin(2));
  Vec3f neg(aabb.GetMax(0), aabb.GetMax(1), aabb.GetMax(2));

  if (plane.A() >= 0)
  {
    pos.x = aabb.GetMax(0);
    neg.x = aabb.GetMin(0);
  }
  if (plane.B() >= 0)
  {
    pos.y = aabb.GetMax(1);
    neg.y = aabb.GetMin(1);
  }
  if (plane.C() >= 0)
  {
    pos.z = aabb.GetMax(2);
    neg.z = aabb.GetMin(2);
  }

  // Pos is corner furthest along in the direction of the plane normal.
  // Neg is the opposite corner. The dist from neg to the plane is the pen depth.
  // If neg dist is positive, the box is in front of the plane.
  Vec3f norm = plane.Normal();
  // Distance from point to plane for pos and neg
  float distP = DotProduct(pos, norm) - plane.D();
  float distN = DotProduct(neg, norm) - plane.D();

  // 4 cases. Intersection if pos and neg corner are on opposite sides of the plane.
  if (distP < 0 && distN < 0)
  {
    return AMJU_BEHIND_PLANE;
  }
  else if (distP < 0) //  && distN >= 0
  {
    Assert(0); // if pos is behind plane, neg should be also
    return AMJU_BEHIND_PLANE;
  }
  else if (distN < 0) // && distP > 0
  {
    *penDepth = -distN;
    return AMJU_INTERSECTING_PLANE;
  }
  else // distN >= 0 && distP >= 0
  {
    return AMJU_IN_FRONT_OF_PLANE;
  }
}

}
