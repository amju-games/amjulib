#ifndef PLANE_H
#define PLANE_H

#include "Tri.h"

namespace Amju
{
enum PlaneResult
{
  AMJU_BEHIND_PLANE,
  AMJU_IN_FRONT_OF_PLANE,
  AMJU_INTERSECTING_PLANE
};

class Plane
{
public:
  Plane();
  Plane(const Tri& t);
  // Construct from 3 coords
  Plane(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2);
  // Construct from normal and dist
  Plane(const Vec3f& n, float d);

  float A() const { return m_a; }
  float B() const { return m_b; }
  float C() const { return m_c; }
  float D() const { return m_d; }

  Vec3f Normal() const { return Vec3f(m_a, m_b, m_c); }
  float Dist(const Vec3f& p) const;

private:
  void Init(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2);
  float m_a, m_b, m_c, m_d;
};
}

#endif

