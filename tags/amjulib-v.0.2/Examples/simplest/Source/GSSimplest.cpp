#include <AmjuGL.h>
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

  static Teapot tp;
  AmjuGL::SetColour(1, 0, 0);
  tp.Draw();
}
}

