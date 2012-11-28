#include <Unproject.h>
// Magic Software triangle-sphere intersect test
#include "Mgc/MgcIntr3DTriSphr.h"
// Mgc point-in-poly test
#include "Mgc/MgcCont2DPointInPolygon.h"
// Mgc distance from line to triangle
#include "Mgc/MgcDist3DLinTri.h"
// Mgc dist from point to triangle
#include "Mgc/MgcDist3DVecTri.h"
#include <CollisionMesh.h>
#include <Line3.h>
#include <Plane.h>
#include "MouseToGroundPos.h"

namespace Amju
{
bool MouseToGroundPos(const CollisionMesh* m, const Vec2f& mouseScreen, Vec3f* groundPos)
{
  Vec3f mouseWorldNear;
  Vec3f mouseWorldFar;

  Unproject(mouseScreen, 0, &mouseWorldNear);
  Unproject(mouseScreen, 1, &mouseWorldFar);
 
  Vec3f dir = mouseWorldFar - mouseWorldNear;

  LineSeg seg(mouseWorldNear, mouseWorldFar);

  // Find all intersecting floor tris
  CollisionMesh::Tris tris;
  Capsule cap(seg, 1.0f); // TODO Radius 

  if (!m->Intersects(cap, &tris))
  {
std::cout << "MouseToGroundPos: Line seg didn't intersect any tris\n";
    return false;
  }

  if (tris.empty())
  {
    // So Intersects() above should have returned false!?
    Assert(0);
    return false;
  }

  Mgc::Vector3 pt1(seg.p0.x, seg.p0.y, seg.p0.z);
  Mgc::Vector3 pt2(seg.p1.x - seg.p0.x, seg.p1.y - seg.p0.y, seg.p1.z - seg.p0.z);
  Mgc::Segment3 s;
  s.Origin() = pt1;
  s.Direction() = pt2;

  // Iterate over tris - find intersection points. Keep track of the closest point to p0.
  Vec3f closest = seg.p1;
  float closestSqDist = (seg.p1 - seg.p0).SqLen();

  int size = tris.size();

#ifdef TERRAIN_DEBUG
std::cout << "Found " << size << " tris....\n";
#endif

  bool foundOne = false;
  Tri theTri;
  for (int i = 0; i < size; i++)
  {
    const Tri& t = tris[i];

    const Vec3f& a = t.m_verts[0];
    const Vec3f& b = t.m_verts[1];
    const Vec3f& c = t.m_verts[2];

    // Skip tris facing away from us. This means the normal will be between 0 and 1.
    Plane plane(a, b, c);
    if (DotProduct(plane.Normal(),  dir) > 0)
    {
      continue;
    }

    // Skip (nearly) vertical polys, so we can't climb up walls
    if (plane.Normal().y < 0.2f) // TODO CONFIG
    {
      continue;
    }

#ifdef TERRAIN_DEBUG
std::cout << "This tri: " << a << " ; " << b << " ; " << c << "\n";
std::cout << "Seg: " << seg.p0 << " - " << seg.p1 << "\n";
#endif

    Mgc::Triangle3 tri;
    tri.Origin() = Mgc::Vector3(a.x, a.y, a.z);
    tri.Edge0() = Mgc::Vector3(b.x - a.x, b.y - a.y, b.z - a.z);
    tri.Edge1() = Mgc::Vector3(c.x - a.x, c.y - a.y, c.z - a.z);

    float q = 0; // parameter along line seg 0..1

#ifdef TERRAIN_DEBUG
    float d =
#endif
    Mgc::SqrDistance(s, tri, &q);
    Vec3f p = seg.p0 + q * (seg.p1 - seg.p0);

#ifdef TERRAIN_DEBUG
std::cout << "SqrDistance intersect: " << p << "\n";

    // Dist d should be zero - the line seg intersects the tri.
std::cout << "d=" << d << "... expecting zero.\n";
#endif

    // Get dist from p0 to p so we get closest tri along line seg.
    float squareDist = (seg.p0 - p).SqLen(); // TODO or should that be p1 ?

    if (squareDist < closestSqDist)
    {
      closestSqDist = squareDist;
      closest = p;
      foundOne = true;
      theTri = t; // remember triangle for later
    }
  }

  // If selected tri is vertical, find the height at the (x, z). 
  m->GetClosestY(Vec2f(closest.x, closest.z), closest.y, &closest.y);  

  *groundPos = closest;

  return foundOne;
}

}


