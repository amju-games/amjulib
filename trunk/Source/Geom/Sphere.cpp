#include "Sphere.h"
#include <Line3.h>

namespace Amju
{
const Vec3f& Sphere::GetCentre() const
{
  return m_centre;
}

float Sphere::GetRadius() const
{
  return m_radius;
}


bool Intersects(const Sphere& s1, const Vec3f& vel1, const Sphere& s2, const Vec3f& vel2, float* timeFirstContact)
{
  // Treat s1 as stationary
  Vec3f relVel = vel2 - vel1;
  LineSeg ls(s2.GetCentre(), s2.GetCentre() + relVel);
  float sqd = ls.SqDist(s1.GetCentre());
  float rsum = s1.GetRadius() + s2.GetRadius();
  float sqrsum = rsum * rsum;
  // If dist from s1 centre to line seg is greater than sum of radii, no intersection
  if (sqd > sqrsum)
  {
    return false;
  }

  // Calc time of first contact
  // TODO

  return true;
}

}
