// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef AMJU_TIME_H_INCLUDED
#define AMJU_TIME_H_INCLUDED

#include <string>

namespace Amju
{
class TimePeriod;
class File;

class Time
{
  friend class TimeRange;

public:
  // Construct a Time containing the current real time.
  static Time Now();
  // Construct a time from constituent secs, min, hours, days, months and years.
  // NB Days start at 1 for the first day of a given month.
  // Months start at 1 for January.
  // Years should be in full (i.e. 4-digit) format, e.g. 1969, 2005.
  static Time MakeTime(int secs, int mins, int hours, int days, int months, int years);

  // Construct from unix timestamp in secs.
  Time(unsigned int secs);

  // Construct from string representation of unix timestamp in secs.
  // Prefer this one as it is upgradeable to 64-bit..? 
  Time(const std::string&);

  // Convert back to unix timestamp
  unsigned int ToSeconds() const;


  // Get constituent parts of a Time.
  int GetSecs() const;
  int GetMins() const;
  int GetHours() const;
  // One-based day, i.e. 1-31 incl.
  int GetDayOfMonth() const;
  // Day of week, Sunday = 0
  int GetDayOfWeek() const;
  // One-based month, i.e. jan==1
  int GetMonths() const;
  // Year returned is in full 4-digit format, e.g. 1969, 2005
  int GetYears() const;
  
  std::string ToString() const;

  std::string ToStringJustDate() const;

  // Time arithmetic: you can add or subtract TimePeriods from Times.
  Time& operator+=(const TimePeriod&);
  Time& operator-=(const TimePeriod&);

  // Get difference between two times.
  // NB the rhs time should be earlier or equal to this time. 
  // If not the two times are swapped.   
  TimePeriod operator-(const Time& rhs) const;

  bool operator==(const Time& rhs) const;

  // Round time down to earlier whole multiple of the given period
  Time& RoundDown(const TimePeriod&);

  // Round up to the next whole multiple of the given period.
  Time& RoundUp(const TimePeriod&);

  // Save and load timestamp
  bool Save(File*);
  bool Load(File*);

  bool operator<(const Time& t) const;
  bool operator<=(const Time& t) const;
  bool operator>(const Time& t) const;
  bool operator>=(const Time& t) const;
 
protected:
  unsigned int m_secs;

};

static const int ONE_MINUTE = 60;
static const int ONE_HOUR = ONE_MINUTE * 60;
static const int ONE_DAY_IN_SECONDS = ONE_HOUR * 24;
static const int ONE_WEEK_IN_SECONDS = ONE_DAY_IN_SECONDS * 7;

}

#endif

