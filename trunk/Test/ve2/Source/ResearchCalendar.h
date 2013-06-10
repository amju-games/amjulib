#ifndef RESEARCH_CALENDAR_H_INCLUDED
#define RESEARCH_CALENDAR_H_INCLUDED

#include <vector>
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
};

typedef std::vector<ResearchDate> ResearchDates;

// Store dates/times of all scheduled research sessions for the study.
// Get this from server, so we can have more than one study/overlapping etc if required.
class ResearchCalendar : public NonCopyable
{
public:
  void Clear();
  const ResearchDates& GetResearchDates() const;
  const ResearchDate* GetToday() const; // could return zero
  const ResearchDate* GetNext() const; // could return zero

  void AddResearchDate(const ResearchDate&);

  int GetDayOnPlanet() const; // get play day number
  
private:
  ResearchDates m_dates;
};

typedef Singleton<ResearchCalendar> TheResearchCalendar;
}

#endif

