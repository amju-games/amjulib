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

  // Get Today cell ?
//  GuiCalendarDayCell* cell = cal->GetCell(Time::Now());
//  cell->SetBgCol(Colour(1, 0, 0, 1));

  const ResearchDates& dates = TheResearchCalendar::Instance()->GetResearchDates();
  if (dates.empty())
  {
    // TODO show message - no sessions are scheduled 
  }
  else
  {
    ResearchDate start = dates[0];
    ResearchDate end = dates[dates.size() - 1];
    Time startTime = start.m_time;
    Time endTime = end.m_time;
    cal->SetStartEndDate(startTime, endTime);

    int n = dates.size();
    for (int i = 0; i < n; i++)
    {
      Time t = dates[i].m_time;
    
      GuiCalendarDayCell* cell = cal->GetCell(t);
      if (cell)
      {
        cell->SetBgCol(Colour(1, 0, 0, 1));
      }
      else
      {
        // TODO Not sure how serious this is
  std::cout << "Date not in range of calendar: " << t.ToString() << "\n";
      }
    }
  }
}

} // namespace
