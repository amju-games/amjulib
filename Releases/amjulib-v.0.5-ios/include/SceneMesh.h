#ifndef SCENE_MESH_H
#define SCENE_MESH_H

#include "SceneNode.h"
#include "ObjMesh.h"

namespace Amju
{
class SceneMesh : public SceneNode
{
public:
  static const char* NAME;
  static SceneNode* Create();

  virtual void Draw();
  virtual void Update();
  virtual bool Load(File*);

  void SetMesh(PObjMesh mesh);

protected:
  // The mesh data - this is a Resource, so is held by the ResourceManager.
  // Multiple leaves can share the same mesh.
  PObjMesh m_mesh;
};
}

#endif


