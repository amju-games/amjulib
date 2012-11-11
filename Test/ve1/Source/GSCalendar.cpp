#include <GuiCalendar.h>
#include <AmjuGL.h>
#include <GuiButton.h>
#include "GSCalendar.h"
#include "ResearchCalendar.h"

namespace Amju
{
static void OnCalOk()
{
  TheGSCalendar::Instance()->GoBack();
}

GSCalendar::GSCalendar()
{
}

void GSCalendar::Update()
{
  GSGui::Update();
}

void GSCalendar::Draw()
{
  DrawCogTestBg();
}

void GSCalendar::Draw2d()
{
  GSGui::Draw2d();
}

void GSCalendar::OnActive()
{
  GSGui::OnActive();
  LoadCogTestBg();

  m_gui = LoadGui("gui-calendar.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetCommand(OnCalOk);

  GuiCalendar* cal = dynamic_cast<GuiCalendar*>(GetElementByName(m_gui, "thecalendar"));
  cal->SetStartEndDate(Time::MakeTime(0, 0, 0, 1, 11, 2012), Time::MakeTime(0, 0, 0, 1, 12, 2012));

  // Get Today cell ?
//  GuiCalendarDayCell* cell = cal->GetCell(Time::Now());
//  cell->SetBgCol(Colour(1, 0, 0, 1));

  const Times& playDays = TheResearchCal::Instance()->GetPlayDates();
  int n = playDays.size();
  for (int i = 0; i < n; i++)
  {
    Time t = playDays[i];
    
    GuiCalendarDayCell* cell = cal->GetCell(t);
    cell->SetBgCol(Colour(1, 0, 0, 1));
  }
}

} // namespace
