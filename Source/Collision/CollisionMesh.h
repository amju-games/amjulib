#ifndef COLLISION_MESH_H
#define COLLISION_MESH_H

#include <vector>
#include <RCPtr.h>
#include "Tri.h"
#include "Matrix.h"
#include "ObjMesh.h"
#include "AABB.h"
#include <Capsule.h>

namespace Amju
{
// Find the y coord for the given (x, z) coord on the given triangle.
// Returns false if the triangle is not facing upwards.
// Returns false if there is no (x, z) coord on the triangle.
bool GetY(const Tri& tri, const Vec2f& xz, float* y);

// TODO SimpleCollisionMesh and OctreeCollisionMesh ?

class CollisionMesh : public RefCounted
{
  // So ObjMesh can easily populate a CollisionMesh
  friend class ObjMesh;

public:
  typedef std::vector<Tri> Tris;

  void Clear();

  // Return true if capsule intersects mesh.
  // Populates vector with intersecting tris. 
  bool Intersects(const Capsule& capsule, Tris* tris) const;

  // Get a height on mesh for (x, z) -- just the first we find.
  bool GetY(const Vec2f& v, float* pY) const;

  // Get height on mesh for (x, z), nearest to given y.
  bool GetClosestY(const Vec2f& v, float y, float* pClosestY) const;

  // Get height on mesh which is closest to given y but equal or lower
  bool GetClosestLowerY(const Vec2f& v, float y, float* pClosestY) const;

  // Get all the triangles in the mesh which intersect the given AABB.
  void GetAllTrisInBox(const AABB& aabb, Tris* pTris) const;

  // Transform all vertices by the given matrix
  void Transform(const Matrix& m);

  // Translate all vertices, cheaper than Transform
  void Translate(const Vec3f& v);

  void Draw();

  void CalcAABB(AABB* aabb);

  const Tris& GetAllTris() const;

private:
  Tris m_tris;
  AABB m_aabb;
};

typedef RCPtr<CollisionMesh> PCollisionMesh;
}

#endif
