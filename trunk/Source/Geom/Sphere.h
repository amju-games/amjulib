#ifndef SPHERE_H
#define SPHERE_H

#include "Vec3.h"

namespace Amju
{
class Sphere
{
public:
  Sphere() { m_radius = 0; }
  Sphere(const Vec3f& centre, float radius) : m_centre(centre), m_radius(radius) {}
 
  const Vec3f& GetCentre() const;
  float GetRadius() const;

  void SetCentre(const Vec3f& c) { m_centre = c; }
  void SetRadius(float r) { m_radius = r; }

private:
  Vec3f m_centre;
  float m_radius;
};
}

#endif
