/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#ifndef FRUSTUM_H_INCLUDED
#define FRUSTUM_H_INCLUDED

#include "Vec3.h"

namespace Amju
{
class Sphere;
class AABB;

class Frustum
{
public:
  Frustum();

  // Creates frustum from projection/modelview matrices.
  void Create();

  enum FrustumResult { AMJU_OUTSIDE, AMJU_INSIDE, AMJU_PART_INSIDE };

  FrustumResult Intersects(const Sphere&) const;

  FrustumResult Intersects(const AABB&) const;

  bool Intersects(const Vec3f&) const;

  // If v is outside the frustum, return the distance to the plane it is
  //  outside of.
  // If v is inside the frustum, returns 0.
//  float DistanceToPoint(const Vec3f&) const;

private:
  FrustumResult SphereInFrustum(float x, float y, float z, float radius) const;

  float frustum[6][4];
};
}
#endif

