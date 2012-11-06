#ifndef RESEARCH_CALENDAR_H_INCLUDED
#define RESEARCH_CALENDAR_H_INCLUDED

#include <vector>
#include <AmjuTime.h>
#include <Singleton.h>

namespace Amju
{
typedef std::vector<Time> Times;

// Store dates/timees of all scheduled research sessions for the study.
// Get this from server, so we can have more than one study/overlapping etc if required.
class ResearchCalendar : public NonCopyable
{
public:
  void Clear();
  const Times& GetPlayDates();
  const Times& GetTestOnlyDates();

  void AddPlayDate(Time);
  void AddTestOnlyDate(Time);

private:
  Times m_play;
  Times m_testOnly;
};

typedef Singleton<ResearchCalendar> TheResearchCal;
}

#endif

