#pragma once

#include <AmjuGL.h>
#include <Material.h>
#include "SceneNode.h"
#include <TriList.h>
#include "Texture.h"

namespace Amju
{
// Convenience base class for meshes created procedurally, e.g. a plane,
//  sphere, etc, rather than loaded from a file.
// TODO Dynamic and Static subclasses?
class SceneNodeProcMesh : public SceneNode
{
public:
  void SetMaterial(Material* m) { m_material = m; }

protected:
  void UseMaterial();

protected:
  // TODO Full Material here, then no need for m_texture
  PTexture m_texture;

  RCPtr<Material> m_material;

  // The tri list - dynamic, static version not required, right?
  RCPtr<TriListDynamic> m_triList;

};
}


