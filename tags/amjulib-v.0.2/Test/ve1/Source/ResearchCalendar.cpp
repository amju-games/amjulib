#include <TimePeriod.h>
#include "ResearchCalendar.h"

namespace Amju
{
void ResearchCalendar::Clear()
{
  m_dates.clear();
}

const ResearchDates& ResearchCalendar::GetResearchDates() const
{
  return m_dates;
}

void ResearchCalendar::AddResearchDate(const ResearchDate& d)
{
  m_dates.push_back(d);
}

const ResearchDate* ResearchCalendar::GetToday() const
{
  Time today(Time::Now());
  today.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));

  for (unsigned int i = 0; i < m_dates.size(); i++)
  {
    if (m_dates[i].m_time == today)
    {
      return &m_dates[i];
    }
  }

  return 0;
}

const ResearchDate* ResearchCalendar::GetNext() const
{
  return 0;
}

int ResearchCalendar::GetDayOnPlanet() const
{
  int d = 0;
  Time today(Time::Now());
  today.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));

  for (unsigned int i = 0; i < m_dates.size(); i++)
  {
    if (m_dates[i].m_play)
    {
      d++;
    }

    if (!(m_dates[i].m_time < today)) // sigh, TODO >=
    {
      return d;
    }
  }
  return d;
}

}


