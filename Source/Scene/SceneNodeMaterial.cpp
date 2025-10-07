// * Amjulib *
// (c) Copyright 2017 Juliet Colman

#include "SceneNodeMaterial.h"

namespace Amju
{
void SceneNodeMaterial::SetMaterial(PMaterial material)
{
}

void SceneNodeMaterial::Draw()
{
  // Use shader, set uniforms
}

bool SceneNodeMaterial::Load(File* f)
{
  if (!SceneNode::Load(f))
  {
    return false;
  }

  return true;
}
}
