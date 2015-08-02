#include "GSRayTrace.h"
#include <AmjuGL.h>

namespace Amju
{
GSRayTrace::GSRayTrace()
{
  m_name = "GPU Ray tracing";
  m_description = "Ray tracing in fragment shader.";
  //m_nextState=...
}

void GSRayTrace::Update()
{
  GSBase::Update();

}

void GSRayTrace::DrawScene()
{
  m_fspp.DrawFullScreenQuad();
}

void GSRayTrace::CreateTweakMenu()
{
  GSBase::CreateTweakMenu();
}

void GSRayTrace::OnActive()
{
  GSBase::OnActive();

  Shader* shader = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/raytrace");
  m_fspp.SetPostProcessShader(shader);
  m_fspp.InitFullScreenQuad();
}

bool GSRayTrace::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSRayTrace::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
