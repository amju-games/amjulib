#include <AmjuGL.h>
#include <AABB.h>
#include <DrawAABB.h>
#include <Colour.h>
#include <Font.h>
#include <GuiText.h>
#include <Timer.h>
#include "GSLighting.h"
#include "GSEnvMap.h"
#include "Teapot.h"
#include "Tweakable.h"

namespace Amju
{
GSLighting::GSLighting()
{
  m_name = "Lighting";
  m_nextState = TheGSEnvMap::Instance();
  m_maxTime = 10.0f; // so awesome
}

void GSLighting::CreateTweakMenu() 
{
  CreateTweakMenu();
  AddTweakable(m_tweaker, new TweakableFloat("Float test", nullptr, 0, 1));
  AddTweakable(m_tweaker, new TweakableFloat("Another test", nullptr, 0, 1));
}

void GSLighting::Update()
{
  GSBase::Update();
}

void GSLighting::DrawScene()
{
  DrawHelp();

  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));

  static float t = 0;
  if (!m_paused)
  {
    t += TheTimer::Instance()->GetDt();
  }
  Vec3f pos(cos(t), 1, sin(t));

  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(pos.x, pos.y, pos.z)); // Light direction

  static float f = 0;
  if (!m_paused)
  {
    f += 0.01f;
  }

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

  // Draw light source
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  static AABB lightAabb(-0.1f, 0.1f, -0.1f, 0.1f, -0.1f, 0.1f);
  AmjuGL::PushMatrix();
  const float dist = 5.0f;
  AmjuGL::Translate(pos.x * dist, 1, pos.z * dist);
  DrawSolidAABB(lightAabb); 
  AmjuGL::PopMatrix();
}

void GSLighting::OnActive()
{
  GSBase::OnActive(); 
}

} // namespace
