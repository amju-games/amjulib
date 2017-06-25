// * Amjulib *
// (c) Copyright 2017 Jason Colman

#include "SceneNodeMaterial.h"

namespace Amju
{
void SceneNodeMaterial::SetTexture(PTexture tex, int index, const std::string& uniformName)
{
}

void SceneNodeMaterial::SetTexture(PTexture tex)
{
  m_tex = tex;
}

void SceneNodeMaterial::SetShader(PShader shader)
{
}

void SceneNodeMaterial::Draw()
{
  // Use shader, set uniforms
  if (m_tex)
  {
    m_tex->UseThisTexture();
  }
}

}
