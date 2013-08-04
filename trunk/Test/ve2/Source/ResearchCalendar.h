#ifndef RESEARCH_CALENDAR_H_INCLUDED
#define RESEARCH_CALENDAR_H_INCLUDED

#include <map>
#include <AmjuTime.h>
#include <Singleton.h>

namespace Amju
{
struct ResearchDate
{
  ResearchDate(Time time, bool cogTest, bool playSingle, bool playMulti) :
    m_time(time), m_cogTest(cogTest), m_playSingle(playSingle), m_playMulti(playMulti) {}

  // Date/time of session
  Time m_time;

  // If true, participant is due to take cog tests
  bool m_cogTest;

  // if true, participant is due to play the game, in single- or multi-player mode.
  bool m_playSingle;
  bool m_playMulti;

  bool operator<(const ResearchDate& rd) const
  {
    return m_time < rd.m_time;
  }

  friend std::ostream& operator<<(std::ostream& os, const ResearchDate& rd);
};

typedef std::map<Time, ResearchDate*> ResearchDates;

// Store dates/times of all scheduled research sessions for the study.
// Get this from server, so we can have more than one study/overlapping etc if required.
class ResearchCalendar : public NonCopyable
{
public:
  void Clear();
  const ResearchDates& GetResearchDates() const;
  const ResearchDate* GetToday() const; // could return zero
  const ResearchDate* GetNext() const; // could return zero
  
  // Get the research date for the given timestamp - could return zero
  ResearchDate* Get(const Time&);

  void AddResearchDate(ResearchDate*);
  
private:
  ResearchDates m_dates;
};

typedef Singleton<ResearchCalendar> TheResearchCalendar;
}

#endif

