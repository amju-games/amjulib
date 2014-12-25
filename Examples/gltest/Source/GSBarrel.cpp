#include "GSBarrel.h"
#include <AmjuGL.h>

namespace Amju
{
GSBarrel::GSBarrel()
{
  //m_nextState=...
}

void GSBarrel::Update()
{
  GSBase::Update();

}

void GSBarrel::DrawScene()
{
//  renderToTexture.Begin();
}

void GSBarrel::Draw2d()
{
}

void GSBarrel::OnActive()
{
}

bool GSBarrel::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSBarrel::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
