#include <AmjuGL.h>
#include <Teapot.h>
#include <Shader.h>
#include <DepthOfField.h>
#include <Vec3.h>
#include "GSDepthOfField.h"
#include "GSShadow.h"

namespace Amju
{
static DepthOfField* dof;

GSDepthOfField::GSDepthOfField()
{
  m_name = "Depth Of Field";
  m_nextState = TheGSShadow::Instance();
  m_maxTime = 200.0f;
}

void GSDepthOfField::Update()
{
  GSBase::Update();
}

static void DrawScene()
{
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D); // not textured
  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR = 1.0f;
  const float FAR = 3000.0f;
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
  static Teapot tp;
  AmjuGL::SetColour(Colour(0.2, 0.2, 1, 1));
  AmjuGL::PushMatrix();
  tp.Draw();

  AmjuGL::SetColour(Colour(1, 0, 0, 1));
  AmjuGL::PushMatrix();
  AmjuGL::Translate(d, 0, d);
  tp.Draw();
  AmjuGL::SetColour(Colour(1, 0, 1, 1));
  AmjuGL::Translate(d, 0, d);
  tp.Draw();
  AmjuGL::PopMatrix();

  AmjuGL::SetColour(Colour(0, 1, 0, 1));
  AmjuGL::PushMatrix();
  AmjuGL::Translate(-d, 0, -d);
  tp.Draw();
  AmjuGL::SetColour(Colour(1, 1, 0, 1));
  AmjuGL::Translate(-d, 0, -d);
  tp.Draw();
  AmjuGL::PopMatrix();

  AmjuGL::PopMatrix();
}

void GSDepthOfField::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));
  
  dof->Draw();
//  DrawScene();
}

void GSDepthOfField::Draw2d()
{
  GSBase::Draw2d();

}

void GSDepthOfField::OnActive()
{
  GSBase::OnActive();

  dof = dynamic_cast<DepthOfField*>(AmjuGL::Create(DepthOfField::DRAWABLE_TYPE_ID));
  Assert(dof);
  dof->SetDrawFunc(DrawScene);
  dof->Init();
}

} // namespace
