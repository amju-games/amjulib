#include "Sphere.h"

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
}
