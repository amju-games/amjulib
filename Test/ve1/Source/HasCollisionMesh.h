#ifndef HAS_COLLISION_MESH_H_INCLUDED
#define HAS_COLLISION_MESH_H_INCLUDED

#include "SceneCollisionMesh.h"

namespace Amju
{
class HasCollisionMesh
{
public:
  virtual ~HasCollisionMesh() {}

  // For collision tests/shadows -- TODO Octree
  CollisionMesh* GetCollisionMesh()
  {
    return m_sceneNode->GetCollisionMesh();
  }

protected:
  // (Octree of) triangles ??
  // Use Obj Mesh, so we can have different materials for different parts of the terrain..?

  // If a regular grid, we don't need anything fancy, you can just divide the coord to get the triangle.

  SceneCollisionMesh* m_sceneNode;

};
}

#endif

