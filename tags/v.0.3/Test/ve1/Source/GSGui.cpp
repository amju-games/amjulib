#include "GSGui.h"
#include <AmjuGL.h>
#include <CursorManager.h>
#include <Timer.h>
#include <EventPoller.h>

namespace Amju
{
GSGui::GSGui()
{
}

void GSGui::Update()
{
  GSBase::Update();
  TextDraw::Update();
}

void GSGui::Draw()
{
  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING | AmjuGL::AMJU_DEPTH_WRITE | AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  static float f = 0;
  AmjuGL::RotateZ(f);
  f += 30.0f * TheTimer::Instance()->GetDt();
  m_bgImage.Draw();  
  AmjuGL::PopAttrib();

  GSBase::Draw();
  TextDraw::Draw();
}

void GSGui::Draw2d()
{
  GSBase::Draw2d();

  if (m_gui)
  {
    m_gui->Draw();
  }

  TheCursorManager::Instance()->Draw();
}

void GSGui::OnActive()
{
  GSBase::OnActive();
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));

  if (!m_bgImage.OpenAndLoad("bgimage.txt"))
  {
std::cout << "Failed to load GUI bg image!\n";
    Assert(0);
  }
}

void GSGui::OnDeactive()
{
  GSBase::OnDeactive();

  // Reload GUI every time we activate. 
  // (This makes it reasonable to add gui elements as listeners when they are created)
  if (TheEventPoller::Instance()->HasListener(m_gui))
  {
    TheEventPoller::Instance()->RemoveListener(m_gui);
  }
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