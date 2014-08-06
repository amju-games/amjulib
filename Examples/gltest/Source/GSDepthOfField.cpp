#include <AmjuGL.h>
#include <Teapot.h>
#include <Shader.h>
#include <Vec3.h>
#include <Timer.h>
#include <ResourceManager.h>
#include <ObjMesh.h>
#include "GSDepthOfField.h"
#include "GSShadow.h"

namespace Amju
{
static ObjMesh* bunny = 0;

GSDepthOfField::GSDepthOfField()
{
  m_name = "Depth Of Field";
  m_nextState = TheGSShadow::Instance();
  m_maxTime = 200.0f;
  m_shader = 0;
}

void GSDepthOfField::Update()
{
  GSBase::Update();
}

static void DrawScene()
{
  AmjuGL::UseShader(0);

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D); // not textured
  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR = 6.0f;
  const float FAR = 20.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR, FAR);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  Vec3f pos(1, 1, 1);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(pos.x, pos.y, pos.z)); // Light direction

  float d = 3.0f;

  AmjuGL::LookAt(3, 5, 12,  3, 0, 0,  0, 1, 0);
  //static Teapot tp;
  AmjuGL::SetColour(Colour(0.2, 0.2, 1, 1));
  AmjuGL::PushMatrix();
  //tp.Draw();
  bunny->Draw();

  AmjuGL::SetColour(Colour(1, 0, 0, 1));
  AmjuGL::PushMatrix();
  AmjuGL::Translate(d, 0, d);
  //tp.Draw();
  bunny->Draw();

  AmjuGL::SetColour(Colour(1, 0, 1, 1));
  AmjuGL::Translate(d, 0, d);
  //tp.Draw();
  bunny->Draw();
  AmjuGL::PopMatrix();

  AmjuGL::SetColour(Colour(0, 1, 0, 1));
  AmjuGL::PushMatrix();
  AmjuGL::Translate(-d, 0, -d);
  //tp.Draw();
  bunny->Draw();
  AmjuGL::SetColour(Colour(1, 1, 0, 1));
  AmjuGL::Translate(-d, 0, -d);
  //tp.Draw();
  bunny->Draw();
  AmjuGL::PopMatrix();

  AmjuGL::PopMatrix();
}

void GSDepthOfField::Draw()
{
  AmjuGL::UseShader(m_shader);
  float dt = TheTimer::Instance()->GetDt();
  static float t = 0;
  t += dt;
  float focus = (sinf(t * 0.25f) + 1.0f) * 0.5f;
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
  m_dof->SetDrawFunc(DrawScene);
  if (!m_dof->Init())
  {
    std::cout << "Failed to init dof.\n";
    Assert(0);
  }

  m_shader = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/dof");
  Assert(m_shader);

std::cout << "Loaded shader for DOF post process effect\n";

  AmjuGL::UseShader(m_shader);
  m_shader->Set("diffuseSampler", (AmjuGL::TextureHandle)0);
  m_shader->Set("depthSampler", (AmjuGL::TextureHandle)1);

  m_dof->SetPostProcessShader(m_shader);

  bunny = (ObjMesh*)TheResourceManager::Instance()->GetRes("bunny_with_normals.obj");
}

} // namespace
