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
  return 0;
}

const ResearchDate* ResearchCalendar::GetNext() const
{
  return 0;
}

}


