#include <math.h>
#include "Plane.h"

namespace Amju
{
Plane::Plane()
{
  m_a = 0;
  m_b = 0;
  m_c = 0;
  m_d = 0;
}

Plane::Plane(const Tri& t)
{
  Init(t.m_verts[0], t.m_verts[1], t.m_verts[2]);
}

Plane::Plane(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
{
  Init(v0, v1, v2);
}

void Plane::Init(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
{
  Vec3f n = CrossProduct(v1 - v0, v2 - v0);
  n.Normalise();
  m_a = n.x;
  m_b = n.y;
  m_c = n.z;
  m_d = DotProduct(v0, n);
}

float Plane::Dist(const Vec3f& p) const
{
  // dist = p.N - D
  return DotProduct(p, Normal()) - m_d;
}
}
