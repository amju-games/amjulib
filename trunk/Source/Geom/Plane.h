#ifndef PLANE_H
#define PLANE_H

#include "Tri.h"

namespace Amju
{
class Plane
{
public:
  Plane();
  Plane(const Tri& t);
  Plane(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2);

  float A() const { return m_a; }
  float B() const { return m_b; }
  float C() const { return m_c; }
  float D() const { return m_d; }

private:
  void Init(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2);
  float m_a, m_b, m_c, m_d;
};
}

#endif

