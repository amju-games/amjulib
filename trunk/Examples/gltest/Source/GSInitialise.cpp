#include "GSInitialise.h"
#include "GSDepthOfField.h"
#include "GSBogusSSAO.h"
#include "GSDepthOfField.h"
#include "GSRenderColourToTexture.h"

namespace Amju
{
GSInitialise::GSInitialise()
{
  m_nextState = TheGSBogusSSAO::Instance();
  m_maxTime = 0.1f;
}

void GSInitialise::Update()
{
  GSBase::Update();
}

void GSInitialise::Draw()
{
  GSBase::Draw();
}

void GSInitialise::Draw2d()
{
}

void GSInitialise::OnActive()
{
}

bool GSInitialise::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSInitialise::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
