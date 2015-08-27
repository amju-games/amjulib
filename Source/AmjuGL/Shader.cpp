#include "Shader.h"

namespace Amju
{
void Uniform::FindLocation() 
{
  if (m_location == -1) 
    m_location = m_shader->FindUniformLocation(m_name); 
}
  
void Attribute::FindLocation() 
{ 
  if (m_location == -1) 
    m_location = m_shader->FindAttribLocation(m_name); 
}

void Shader::AddAttribute(Attribute* av) 
{ 
  av->SetShader(this); 
  m_attribs.push_back(av); 
}

void Shader::AddUniform(Uniform* uv) 
{
  uv->SetShader(this); 
  m_uniforms.push_back(uv); 
}

void Shader::SetVariables()
{
  for (auto sh : m_uniforms)
  {
    sh->SetUniform();
  }

  for (auto sh : m_attribs)
  {
    sh->SetAttrib();
  }
}

}

