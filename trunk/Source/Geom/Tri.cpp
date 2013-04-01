#include <AmjuFirst.h>
#include "Tri.h"
#include "AmjuGL.h"
#include <AmjuFinal.h>

namespace Amju
{
void Tri::Transform(const Matrix& m)
{
  for (int i = 0; i < 3; i++)
  {
    m_verts[i] = m_verts[i] * m; // TODO no *= ??
  }
}

void Tri::Translate(const Vec3f& v)
{
  for (int i = 0; i < 3; i++)
  {
    m_verts[i] += v;
  }
}

void Tri::Draw()
{
  AmjuGL::Vec3 v0(m_verts[0].x, m_verts[0].y, m_verts[0].z);
  AmjuGL::Vec3 v1(m_verts[1].x, m_verts[1].y, m_verts[1].z);
  AmjuGL::Vec3 v2(m_verts[2].x, m_verts[2].y, m_verts[2].z);

  AmjuGL::DrawLine(v0, v1);
  AmjuGL::DrawLine(v1, v2);
  AmjuGL::DrawLine(v2, v0);
}
}
