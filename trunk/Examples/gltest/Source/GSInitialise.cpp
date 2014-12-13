#include "GSInitialise.h"
#include "GSDepthOfField.h"
#include "GSBogusSSAO.h"
#include "GSDepthOfField.h"
#include "GSRenderColourToTexture.h"
#include "GSRenderDepthToTexture.h"
#include "GSShadow2.h"
#include "GSShaderWave.h"

namespace Amju
{
GSInitialise::GSInitialise()
{
  //m_nextState = TheGSBogusSSAO::Instance();
  //m_nextState = TheGSRenderColourToTexture::Instance();
  //m_nextState = TheGSRenderDepthToTexture::Instance();
  //m_nextState = TheGSDepthOfField::Instance();
  m_nextState = TheGSShadow2::Instance();
  //m_nextState = TheGSShaderWave::Instance();
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
  GSBase::Draw2d();
}

void GSInitialise::OnActive()
{
  GSBase::OnActive();
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
