// * Amjulib *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Material.h>
#include <SceneNode.h>

namespace Amju
{
// * SceneNodeMaterial *
// Scene node which sets a material.
class SceneNodeMaterial : public SceneNode
{
public:
  void SetMaterial(PMaterial material); 

  virtual bool Load(File*) override;
  // Use this material. Special materials can subclass if required.
  virtual void Draw() override;

protected:
  PMaterial m_material;
};

using PSceneNodeMaterial = RCPtr<SceneNodeMaterial>;
}

