#include "GSLighting.h"
#include <AmjuGL.h>
#include <AABB.h>
#include <DrawAABB.h>
#include <Game.h>
#include <Colour.h>
#include <Font.h>
#include "Teapot.h"

namespace Amju
{
GSLighting::GSLighting()
{
  //m_nextState=...
}

void GSLighting::Update()
{
//  GSBase::Update();

}

void GSLighting::Draw()
{
  GSBase::Draw();

  TheGame::Instance()->SetClearColour(Colour(0, 0, 0, 1));

  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(1, 1, 1)); // Light direction

  static float f = 0;
  f += 0.2f;

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
  AmjuGL::SetColour(0, 0, 0, 1);
  static Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/arial-font.font");
  Assert(font);
  font->Print(-1, -0.8f, "Lighting");
  AmjuGL::SetColour(1, 1, 1, 1);
}

void GSLighting::OnActive()
{
}

bool GSLighting::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSLighting::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
