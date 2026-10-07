// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
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
