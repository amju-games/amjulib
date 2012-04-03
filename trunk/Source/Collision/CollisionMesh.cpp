#include "CollisionMesh.h"
#include "Vec2.h"
#include "Plane.h"
// Magic Software triangle-sphere intersect test
#include "Mgc/MgcIntr3DTriSphr.h"
// Mgc point-in-poly test
#include "Mgc/MgcCont2DPointInPolygon.h"
// Mgc distance from line to triangle
#include "Mgc/MgcDist3DLinTri.h"
// Mgc dist from point to triangle
#include "Mgc/MgcDist3DVecTri.h"

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

bool CollisionMesh::Intersects(const Capsule& cap, Tris* tris) const
{
  const Vec3f& p0 = cap.m_lineseg.p0;
  Vec3f dir = cap.m_lineseg.p1 - cap.m_lineseg.p0;

  Mgc::Segment3 seg;
  seg.Origin() = Mgc::Vector3(p0.x, p0.y, p0.z);
  seg.Direction() = Mgc::Vector3(dir.x, dir.y, dir.z);

  float r2 = cap.m_radius * cap.m_radius;
  // For each tri, get dist from capsule line seg to tri, compare with capsule radius.
  bool ret = false;
  for (Tris::const_iterator it = m_tris.begin(); it != m_tris.end(); ++it)
  {
    const Tri& t = *it;

    const Vec3f& a = t.m_verts[0];
    const Vec3f& b = t.m_verts[1];
    const Vec3f& c = t.m_verts[2];

    Mgc::Triangle3 tri;
    tri.Origin() = Mgc::Vector3(a.x, a.y, a.z);
    tri.Edge0() = Mgc::Vector3(b.x - a.x, b.y - a.y, b.z - a.z);
    tri.Edge1() = Mgc::Vector3(c.x - a.x, c.y - a.y, c.z - a.z);

    float squareDist = Mgc::SqrDistance(seg, tri);

    if (squareDist <= r2)
    {
      tris->push_back(t);
      ret = true;
    }
  }
  return ret;
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

bool CollisionMesh::GetClosestY(const Vec2f& v, float y, float* pClosestY) const
{
  float bestDist = 99999.9f;
  float bestY = y;
  bool ok = false;
  for (Tris::const_iterator it = m_tris.begin(); it != m_tris.end(); ++it)
  {
    const Tri& t = *it;
    if (PointXZInTri(t, v.x, v.y))
    {
      Plane p(t);
      // Is tri facing upwards ?
      if (p.B() > 0.1f) // ?
      {
        ok = true;
        // Plane eq: Ax + By + Cz = D
        // =>        y = (Ax + Cz + D) / -B;
        float thisY = (p.A() * v.x + p.C() * v.y - p.D()) / -p.B();
        float dist = fabs(thisY - y);
        if (dist < bestDist)
        {
          bestDist = dist;
          bestY = thisY;
        } 
      }
    }
  }
  *pClosestY = bestY;
  return ok;
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

