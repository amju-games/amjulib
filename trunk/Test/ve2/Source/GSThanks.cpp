#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <GuiButton.h>
#include <Game.h>
#include <TimePeriod.h>
#include "GSThanks.h"
#include "GSCalendar.h"
#include "ResearchCalendar.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnThanksOk()
{
  TheGSThanks::Instance()->GoBack();
}

static void OnThanksCal()
{
  TheGame::Instance()->SetCurrentState(TheGSCalendar::Instance());
}

GSThanks::GSThanks()
{
}

void GSThanks::Update()
{
  GSGui::Update();

}

void GSThanks::Draw()
{
  GSGui::Draw();

}

void GSThanks::Draw2d()
{
  GSGui::Draw2d();
}

void GSThanks::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-thanks.txt");
  Assert(m_gui);

  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "thanks-ok-button");
  ok->SetCommand(OnThanksOk);

  GuiButton* cal = (GuiButton*)GetElementByName(m_gui, "thanks-calendar-button");
  cal->SetCommand(OnThanksCal);

  GuiText* text = (GuiText*)GetElementByName(m_gui, "long-text");

  const ResearchDates& dates = TheResearchCalendar::Instance()->GetResearchDates();
  if (!dates.empty())
  {
    int numDays = dates.size();
    // map -- sorted std::sort(dates.begin(), dates.end());
    // This is a bit dodgy - TODO make member func of ResearchCalendar
    TimePeriod period = dates.rbegin()->first - dates.begin()->first;
    int weeks = period.ToSeconds() / ONE_WEEK_IN_SECONDS;

    text->SetText("Thanks for participating in this research project! I hope you find it enjoyable.\n\n"
      "You are asked to play this game a total of " + ToString(numDays) + " times, over the course of " +
      ToString(weeks) + " weeks. I will show you a calendar of when to play."
      "\n\nWould you like to see the calendar now?");
  }
}

} // namespace
