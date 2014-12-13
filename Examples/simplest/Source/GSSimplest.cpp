#include <AmjuGL.h>
#include <Timer.h>
#include <Teapot.h>
#include "GSSimplest.h"

namespace Amju
{
void GSSimplest::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetPerspectiveProjection(45, 1, 1, 100);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::LookAt(0, 5, 10,   0, 0, 0,   0, 1, 0);

  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f),
    AmjuGL::LightColour(0.3f, 0.3f, 0.3f),
    AmjuGL::LightColour(0.7f, 0.7f, 0.3f),
    AmjuGL::LightColour(1.0f, 1.0f, 1.0f),
    AmjuGL::Vec3(1, 1, 1));

  static Teapot tp;
  AmjuGL::SetColour(1, 1, 0);

  static float rot = 0;
  float dt = TheTimer::Instance()->GetDt();
  rot += 100.0f * dt;
  AmjuGL::RotateY(rot);

  tp.Draw();
}
}

