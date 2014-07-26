#include "GSPatchwork.h"
#include "Patchwork.h"

namespace Amju
{
static Patchwork patchwork;

GSPatchwork::GSPatchwork()
{
  //m_nextState=...
}

void GSPatchwork::Update()
{
  GSBase::Update();

}

void GSPatchwork::Draw()
{
  AmjuGL::SetClearColour(Colour(0.5f, 1, 1, 1));

  /// NOT: GSBase::Draw();

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEARDIST = 1.0f; // NEAR and FAR are #defined in Windows, nice one.
  const float FARDIST = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEARDIST, FARDIST);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::LookAt(0, 2.4, 4.0,  0, 0, -10.0,  0, 1, 0);
  
  patchwork.Draw();
}

void GSPatchwork::Draw2d()
{
}

void GSPatchwork::OnActive()
{
  patchwork.Init();
}

bool GSPatchwork::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSPatchwork::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
