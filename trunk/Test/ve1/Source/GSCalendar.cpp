#include <GuiCalendar.h>
#include <AmjuGL.h>
#include "GSCalendar.h"

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

  GuiCalendar* cal = dynamic_cast<GuiCalendar*>(GetElementByName(m_gui, "thecalendar"));
  cal->SetStartEndDate(Time::MakeTime(0, 0, 0, 1, 11, 2012), Time::MakeTime(0, 0, 0, 1, 12, 2012));
}

} // namespace
