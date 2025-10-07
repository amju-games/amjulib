// * Amjulib *
// (c) Copyright 2000-2017 Juliet Colman

#pragma once

#include <Shader.h>
#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecShader *
// Gui decorator which sets the shader for descendant nodes. Also sets a named uniform
//  to the current animation value. This lets us add special shaders which can animate.
class GuiDecShader : public GuiDecorator
{
public:
  static const char* NAME;

  GuiDecShader* Clone() override { return new GuiDecShader(*this); }

  virtual bool Load(File*) override;
  virtual void Animate(float animValue) override;
  virtual void Draw() override;

protected:
  virtual void SetShaderUniform();

protected:
  Shader* m_shader = nullptr; // Resource, so owned by Res Manager, right?

  // Name of uniform to set to m_uniformValue;
  std::string m_uniformName; 

  float m_uniformValue = 0;
};
}

