#include "GSLoginWaiting.h"
#include <AmjuGL.h>

namespace Amju
{
GSLoginWaiting::GSLoginWaiting()
{
  //m_nextState=...
}

void GSLoginWaiting::Update()
{
  GSBase::Update();

}

void GSLoginWaiting::Draw()
{
  GSBase::Draw();

}

void GSLoginWaiting::Draw2d()
{
}

void GSLoginWaiting::OnActive()
{
  GSBase::OnActive();
}

bool GSLoginWaiting::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSLoginWaiting::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
