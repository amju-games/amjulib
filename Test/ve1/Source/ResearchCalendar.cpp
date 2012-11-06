#include "ResearchCalendar.h"

namespace Amju
{
void ResearchCalendar::Clear()
{
  m_play.clear();
  m_testOnly.clear();
}

const Times& ResearchCalendar::GetPlayDates()
{
  return m_play;
}

const Times& ResearchCalendar::GetTestOnlyDates()
{
  return m_testOnly;
}

void ResearchCalendar::AddPlayDate(Time t)
{
  m_play.push_back(t);
}

void ResearchCalendar::AddTestOnlyDate(Time t)
{
  m_testOnly.push_back(t);
}

}


