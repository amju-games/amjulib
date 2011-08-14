#include "GSLighting.h"
#include <AmjuGL.h>
#include <AABB.h>
#include <DrawAABB.h>
#include <Game.h>
#include <Colour.h>
#include <Font.h>
#include <Screenshot.h>
#include <Teapot.h>

namespace Amju
{
GSLighting::GSLighting()
{
}

void GSLighting::Update()
{
}

void GSLighting::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR = 1.0f;
  const float FAR = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR, FAR);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::LookAt(0, 5, 10,  0, 0, 0,  0, 1, 0);

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
  static Teapot tp;

  AmjuGL::PushMatrix();
  AmjuGL::RotateY(f);
  AmjuGL::Scale(2, 2, 2);
  //DrawSolidAABB(aabb);
  tp.Draw();

  AmjuGL::PopMatrix();
}

void GSLighting::Draw2d()
{
}

void GSLighting::OnActive()
{
}

bool GSLighting::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSLighting::OnButtonEvent(const ButtonEvent& be)
{
  if (be.isDown && be.button == AMJU_BUTTON_A)
  {
    SaveScreenshot("test.png", 4);
    UploadScreenshot("test.png", "www.amju.com/cgi-bin/img-upload.pl?playerid=666");
  }
  return false;
}

bool GSLighting::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (!mbe.isDown)
  {
    // Save screenshot
    SaveScreenshot("test.png", 4);

    UploadScreenshot("test.png", "www.amju.com/cgi-bin/img-upload.pl?playerid=1");
  }

  return false;
}
} // namespace
