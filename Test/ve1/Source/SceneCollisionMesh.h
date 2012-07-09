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

  void CalcCollisionMesh(ObjMesh* mesh) { mesh->CalcCollisionMesh(m_collMesh); }

  virtual void Draw()
  {
    AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING);
    AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
    SceneMesh::Draw();
    AmjuGL::PopAttrib();
  }

private:
  PCollisionMesh m_collMesh;
};

}

#endif

