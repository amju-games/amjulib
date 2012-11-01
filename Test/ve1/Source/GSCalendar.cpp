#include "GSCalendar.h"
#include <AmjuGL.h>

namespace Amju
{
GSCalendar::GSCalendar()
{
}

void GSCalendar::Update()
{
  GSGui::Update();

}

void GSCalendar::Draw()
{
  GSGui::Draw();

}

void GSCalendar::Draw2d()
{
  GSGui::Draw2d();
}

void GSCalendar::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-calendar.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
