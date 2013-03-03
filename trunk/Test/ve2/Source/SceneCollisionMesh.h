#ifndef SCENE_COLLISION_MESH_H_INCLUDED
#define SCENE_COLLISION_MESH_H_INCLUDED

#include <SceneMesh.h>
#include <CollisionMesh.h>
#include <AmjuGL.h>

namespace Amju
{
class SceneCollisionMesh : public SceneMesh
{
public:
  SceneCollisionMesh() : m_collMesh(new CollisionMesh) {}

  CollisionMesh* GetCollisionMesh() { return m_collMesh; }

  void CalcCollisionMesh() { m_mesh->CalcCollisionMesh(m_collMesh); }

  virtual void Draw();

private:
  PCollisionMesh m_collMesh;
};

}

#endif

