#include "GSGui.h"
#include <AmjuGL.h>
//#include <CursorManager.h>

namespace Amju
{
GSGui::GSGui()
{
  //m_nextState=...
}

void GSGui::Update()
{
  GSBase::Update();

}

void GSGui::Draw()
{
  GSBase::Draw();

}

void GSGui::Draw2d()
{
  if (m_gui)
  {
    m_gui->Draw();
  }

//  TheCursorManager::Instance()->Draw();
}

void GSGui::OnActive()
{
  GSBase::OnActive();
}

bool GSGui::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSGui::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
