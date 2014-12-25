#include <AmjuGL.h>
#include <Teapot.h>
#include <Shader.h>
#include <Vec3.h>
#include <Timer.h>
#include <ResourceManager.h>
#include "GSDepthOfField.h"
#include "GSBogusSSAO.h"
#include "GSShadow.h"

namespace Amju
{
GSDepthOfField::GSDepthOfField()
{
  m_name = "Depth Of Field";
  //m_nextState = TheGSShadow::Instance();
  m_nextState = TheGSBogusSSAO::Instance();
  m_maxTime = 10.0f;
  m_shader = 0;
}

void GSDepthOfField::Update()
{
  GSBase::Update();
}

void GSDepthOfField::DrawScene()
{
  AmjuGL::UseShader(m_shader);
  float dt = TheTimer::Instance()->GetDt();
  static float t = 0;
  t += dt;
  float focus = (sinf(t * 2.0f) + 1.0f) * 0.5f;
  
  m_shader->Set("focus", focus);

  m_dof->Draw();

  AmjuGL::UseShader(0);
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));
}

void GSDepthOfField::Draw2d()
{
  GSBase::Draw2d();
}

void GSDepthOfField::OnActive()
{
  GSBase::OnActive();

  m_dof = new DepthOfField;
  m_dof->SetDrawFunc(DrawBunnyScene);
  m_shader = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/dof");
  Assert(m_shader);

std::cout << "Loaded shader for DOF post process effect\n";

  AmjuGL::UseShader(m_shader);
  m_shader->Set("diffuseSampler", (AmjuGL::TextureHandle)0);
  m_shader->Set("depthSampler", (AmjuGL::TextureHandle)1);

  m_dof->SetPostProcessShader(m_shader);

  if (!m_dof->Init())
  {
    std::cout << "Failed to init dof.\n";
    Assert(0);
  }
}

} // namespace
