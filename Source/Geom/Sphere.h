#ifndef SPHERE_H
#define SPHERE_H

#include "Vec3.h"

namespace Amju
{
class Sphere
{
public:
  Sphere();
  Sphere(const Vec3f& centre, float radius) : m_centre(centre), m_radius(radius) {}
  void Draw();

private:
  Vec3f m_centre;
  float m_radius;
};
}

#endif
