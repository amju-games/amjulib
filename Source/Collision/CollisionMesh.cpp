#include "CollisionMesh.h"
#include "Vec2.h"
#include "Plane.h"

namespace Amju
{
bool PointXZInTri(const Tri& t, float x, float z)
{
  // 2D point-in-tri test
  // See www.blackpawn.com/texts/pointinpoly/default.html
  Vec2f A = Vec2f(t.m_verts[0].x, t.m_verts[0].z);
  Vec2f B = Vec2f(t.m_verts[1].x, t.m_verts[1].z);
  Vec2f C = Vec2f(t.m_verts[2].x, t.m_verts[2].z);
  Vec2f v0 = C - A;
  Vec2f v1 = B - A;
  Vec2f v2 = Vec2f(x, z) - A;

  float dot00 = DotProduct(v0, v0);
  float dot01 = DotProduct(v0, v1);
  float dot02 = DotProduct(v0, v2);
  float dot11 = DotProduct(v1, v1);
  float dot12 = DotProduct(v1, v2);

  float denom = (dot00 * dot11 - dot01 * dot01);
  if (denom == 0)
  {
    return false;
  }
  float invDenom = 1.0f / denom;
  float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
  float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

  return (u >= 0) && (v >= 0) && (u + v <= 1.0f);
}

bool GetY(const Tri& tri, const Vec2f& v, float* y)
{
  Plane p(tri);
  // Is tri facing upwards ?
  if (p.B() > 0.001f) 
  {
    // Plane eq: Ax + By + Cz = D
    // =>        y = (Ax + Cz + D) / -B;        
    *y = (p.A() * v.x + p.C() * v.y - p.D()) / -p.B();
    return true;
  }
  return false;
}

// Return true if box and tri intersect
bool TriInBox(const AABB& a, const Tri& t)
{
  // Accurate test using SAT is here:
  // http://www.cs.lth.se/home/Tomas_Akenine_Moller/code/tribox3.txt
  // This just creates an AABB around the tri and checks for
  // AABB-AABB intersection - will generate false positives.
  // TODO Use accurate version, check if faster overall
  AABB b(
    t.m_verts[0].x, t.m_verts[0].x, 
    t.m_verts[0].y, t.m_verts[0].y, 
    t.m_verts[0].z, t.m_verts[0].z);
  b.SetIf(t.m_verts[1].x, t.m_verts[1].y, t.m_verts[1].z);
  b.SetIf(t.m_verts[2].x, t.m_verts[2].y, t.m_verts[2].z);

  return b.Intersects(a);
}

void CollisionMesh::GetAllTrisInBox(const AABB& aabb, Tris* pTris) const
{
  // Make enough space to add all our tris to the result...
  // Avoid multiple reallocations, but wastes mem
  pTris->reserve(pTris->size() + m_tris.size());

  for (Tris::const_iterator it = m_tris.begin(); it != m_tris.end(); ++it)
  {
    const Tri& t = *it;
    if (TriInBox(aabb, t))
    {
      pTris->push_back(t);
    }
  }
}

const CollisionMesh::Tris& CollisionMesh::GetAllTris() const
{
  return m_tris;
}

bool CollisionMesh::GetY(const Vec2f& v, float* pY) const
{
  //bool found = false;
  for (Tris::const_iterator it = m_tris.begin(); it != m_tris.end(); ++it)
  {
    const Tri& t = *it;
    if (PointXZInTri(t, v.x, v.y))
    {
      // TODO Use GetY() above
      // TODO find best
      //found = true;
      Plane p(t);
      // Is tri facing upwards ?
      if (p.B() > 0.1f) // ?
      {
        // Plane eq: Ax + By + Cz = D
        // =>        y = (Ax + Cz + D) / -B;        
        *pY = (p.A() * v.x + p.C() * v.y - p.D()) / -p.B();
        return true;
      }
    }
  }
  return false;
}

void CollisionMesh::Transform(const Matrix& m)
{
  for (Tris::iterator it = m_tris.begin(); it != m_tris.end(); ++it)
  {
    Tri& t = *it;
    t.Transform(m);
  }
}

void CollisionMesh::Translate(const Vec3f& v)
{
  for (Tris::iterator it = m_tris.begin(); it != m_tris.end(); ++it)
  {
    Tri& t = *it;
    t.Translate(v);
  }
}

void CollisionMesh::Draw()
{
  for (Tris::iterator it = m_tris.begin(); it != m_tris.end(); ++it)
  {
    Tri& t = *it;
    t.Draw();
  }
}

void CollisionMesh::Clear()
{
  m_tris.clear();
}

void CollisionMesh::CalcAABB(AABB* res)
{
  if (m_tris.empty())
  {
    return;
  }

  AABB aabb(
    m_tris[0].m_verts[0].x, m_tris[0].m_verts[0].x, 
    m_tris[0].m_verts[0].y, m_tris[0].m_verts[0].y, 
    m_tris[0].m_verts[0].z, m_tris[0].m_verts[0].z);

  for (Tris::iterator it = m_tris.begin(); it != m_tris.end(); ++it)
  {
    Tri& t = *it;

    aabb.SetIf(t.m_verts[0].x, t.m_verts[0].y, t.m_verts[0].z);
    aabb.SetIf(t.m_verts[1].x, t.m_verts[1].y, t.m_verts[1].z);
    aabb.SetIf(t.m_verts[2].x, t.m_verts[2].y, t.m_verts[2].z);
  }
  *res = aabb;
}
}

