// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef AMJU_TIME_PERIOD_H_INCLUDED
#define AMJU_TIME_PERIOD_H_INCLUDED

#include <string>

namespace Amju
{
// Time period, stored in seconds.
// Examples: 1 day, 2 weeks, 3 seconds, etc.
// I.e. there is no absolute date/time associated with the period; it's like
// the difference between two timestamps.
class TimePeriod
{
  friend class Time;

public:
  TimePeriod(unsigned int seconds = 0);
  std::string ToString() const;
  unsigned int ToSeconds() const;

  TimePeriod& operator+=(const TimePeriod&);

  bool operator<(const TimePeriod&) const;
  bool operator==(const TimePeriod&) const;
  bool operator>(const TimePeriod&) const; 
  
protected:
  unsigned int m_secs;

};
}

#endif


