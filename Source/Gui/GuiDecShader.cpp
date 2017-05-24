// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include "GuiDecShader.h"

namespace Amju
{
const char* GuiDecShader::NAME = "shader";

bool GuiDecShader::Load(File* f)
{
  // Shader name - prefix, which has -frag, -vert etc added.
  std::string shaderName;
  if (!f->GetDataLine(&shaderName))
  {
    f->ReportError("Expected shader name");
    return false;
  }
  m_shader = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/" + shaderName);
  if (!m_shader)
  {
    f->ReportError("Failed to load GUI shader: " + shaderName);
    Assert(0);
  }

  // Uniform name
  if (!f->GetDataLine(&m_uniformName))
  {
    f->ReportError("Expected uniform name");
    return false;
  }

  return true;
}

void GuiDecShader::Animate(float animValue)
{
  m_uniformValue = animValue;
}

void GuiDecShader::Draw()
{
  Shader* sh = AmjuGL::GetCurrentShader();

  AmjuGL::UseShader(m_shader);
  SetShaderUniform();
  GuiDecorator::Draw();

  AmjuGL::UseShader(sh);
}

void GuiDecShader::SetShaderUniform()
{
  m_shader->Set(m_uniformName, m_uniformValue);
}
}


