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

  virtual void CalcCollisionMesh(CollisionMesh* pCollMesh) const override;
  
  virtual void CalcBoundingVol() override;

  void SetMesh(PObjMesh mesh);
  const ObjMesh* GetMesh() const;

protected:
  // Check materials for any which require blending - set flag on this node.
  void SetBlendFlag();

protected:
  // The mesh data - this is a Resource, so is held by the ResourceManager.
  // Multiple leaves can share the same mesh.
  PObjMesh m_mesh;
};
  
// Obj mesh consisting of a single group and no material, with a separate material which is
//  used when we draw the mesh.
class SceneMeshMaterial : public SceneMesh
{
public:
  static const char* NAME;
  
  virtual bool Load(File*);
  virtual void Draw();
  
  void SetMaterial(const Material& m);
  
protected:
  Material m_material;
};
}

#endif


