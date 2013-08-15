#include <AmjuGL.h>
#include <AABB.h>
#include <DrawAABB.h>
#include <Colour.h>
#include <Font.h>
#include <GuiText.h>
#include "GSLighting.h"
#include "GSEnvMap.h"
#include "Teapot.h"

namespace Amju
{
GSLighting::GSLighting()
{
  m_name = "Lighting";
  m_nextState = TheGSEnvMap::Instance();
}

void GSLighting::Update()
{
  GSBase::Update();
}

void GSLighting::Draw()
{
  GSBase::Draw();

  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));

  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(1, 1, 1)); // Light direction

  static float f = 0;
  f += 0.01f;

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  static AABB aabb(-1, 1, -1, 1, -1, 1);

  AmjuGL::PushMatrix();
  AmjuGL::Translate(-2.0f, 0, 0);
  AmjuGL::RotateY(f);
  DrawSolidAABB(aabb);
  AmjuGL::PopMatrix();

  static Teapot tp;

  AmjuGL::PushMatrix();
  AmjuGL::Translate(2.0f, 0, 0);
  AmjuGL::RotateY(f);
  tp.Draw();
  AmjuGL::PopMatrix();
}

void GSLighting::Draw2d()
{
  GSBase::Draw2d();
}

void GSLighting::OnActive()
{
}

} // namespace
