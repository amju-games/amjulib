#ifndef COLLISION_MESH_H
#define COLLISION_MESH_H

#include <vector>
#include "Tri.h"
#include "Matrix.h"
#include "ObjMesh.h"
#include "AABB.h"

namespace Amju
{
// Find the y coord for the given (x, z) coord on the given triangle.
// Returns false if the triangle is not facing upwards.
// Returns false if there is no (x, z) coord on the triangle.
bool GetY(const Tri& tri, const Vec2f& xz, float* y);

class CollisionMesh
{
  // So ObjMesh can easily populate a CollisionMesh
  friend class ObjMesh;

public:
  typedef std::vector<Tri> Tris;

  void Clear();

  // Get height on mesh for (x, z), nearest to given y.
  bool GetY(const Vec3f& v, float* pY) const;

  // Get all the triangles in the mesh which intersect the given AABB.
  void GetAllTrisInBox(const AABB& aabb, Tris* pTris) const;

  // Transform all vertices by the given matrix
  void Transform(const Matrix& m);

  // Translate all vertices, cheaper than Transform
  void Translate(const Vec3f& v);

  void Draw();

  void CalcAABB(AABB* aabb);

private:
  Tris m_tris;
};
}

#endif
