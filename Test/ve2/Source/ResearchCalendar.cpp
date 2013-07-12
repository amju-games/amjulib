#include <AmjuFirst.h>
#include <TimePeriod.h>
#include "ResearchCalendar.h"
#include <AmjuFinal.h>

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

void ResearchCalendar::AddResearchDate(ResearchDate* d)
{
  ResearchDates::iterator it = m_dates.find(d->m_time);
  
  if (it == m_dates.end())
  {
    m_dates[d->m_time] = d;
  }
  else
  {
    std::cout << "Duplicated research date, so overwriting values...\n";
    if (d->m_cogTest)
    {
      it->second->m_cogTest = true;
    }
    if (d->m_playMulti)
    {
      it->second->m_playMulti = true;
    }
    if (d->m_playSingle)
    {
      it->second->m_playSingle = true;
    }
  }
}

const ResearchDate* ResearchCalendar::GetToday() const
{
  Time today(Time::Now());
  today.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));

  for (ResearchDates::const_iterator it = m_dates.begin(); it != m_dates.end(); ++it)
  {
    if (it->first == today)
    {
      return it->second;
    }
  }

  return 0;
}

const ResearchDate* ResearchCalendar::GetNext() const
{
  return 0;
}

}


