// * Amjulib *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Texture.h>
#include <Shader.h>

namespace Amju
{
// * SceneNodeMaterial *
// Material which is set on a SceneNode.
// Materials contain multiple textures, colours and float values, and a shader.
// The textures and colours etc are used by the shader.
class SceneNodeMaterial : public RefCounted
{
public:
  void SetTexture(PTexture tex); // basic: just diffuse
  void SetTexture(PTexture tex, int index, const std::string& uniformName);
  void SetShader(PShader shader);

  // Use this material. Special materials can subclass if required.
  virtual void Draw();

protected:
  PTexture m_tex;
};

using PSceneNodeMaterial = RCPtr<SceneNodeMaterial>;
}

