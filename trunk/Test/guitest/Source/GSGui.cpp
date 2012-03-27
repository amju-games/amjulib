#include "GSGui.h"
#include <iostream>
#include <AmjuGL.h>
#include <CursorManager.h>

namespace Amju
{
GSGui::GSGui()
{
}

void GSGui::Update()
{
}

void GSGui::Draw()
{
}

void GSGui::Draw2d()
{
  if (m_gui)
  {
    m_gui->Draw();
  }

  TheCursorManager::Instance()->Draw();
}

void GSGui::OnActive()
{
  GameState::OnActive();
}

void GSGui::OnDeactive()
{
  // Reload GUI every time we activate. 
  // (This makes it reasonable to add gui elements as listeners when they are created)
  m_gui = 0;
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
