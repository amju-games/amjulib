#include "GSLighting.h"
#include <AmjuGL.h>
#include <AABB.h>
#include <DrawAABB.h>
#include <Game.h>
#include <Colour.h>
#include <Font.h>

namespace Amju
{
GSLighting::GSLighting()
{
  //m_nextState=...
}

void GSLighting::Update()
{
  GSBase::Update();

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
  AmjuGL::RotateY(f);

  AABB aabb(-1, 1, -1, 1, -1, 1);
  //m_tex->UseThisTexture(); // not required, we set texture in shader

  DrawSolidAABB(aabb);
}

void GSLighting::Draw2d()
{
  //static Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/arial-font.font");
  //Assert(font);
  //font->Print(-1, -0.8f, "Lighting");
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
