#ifndef TRI_H
#define TRI_H

#include "Vec3.h"
#include "Matrix.h"

namespace Amju
{
struct Tri
{
  Tri() {}
  Tri(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2) 
  {
    m_verts[0] = v0;
    m_verts[1] = v1;
    m_verts[2] = v2;
  }

  void Transform(const Matrix&);
  void Translate(const Vec3f&);
  void Draw();

  Vec3f m_verts[3];

  Vec3f CalcNormal() const;
  
  Vec3f CalcCentre() const
  {
    return (m_verts[0] + m_verts[1] + m_verts[2]) * (1.0f / 3.0f);
  }
};
}

#endif

