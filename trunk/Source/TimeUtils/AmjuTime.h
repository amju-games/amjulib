/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: AmjuTime.h,v $
Revision 1.2  2006/01/16 14:08:52  jay
Add Time classes to trunk

Revision 1.1.2.1  2005/08/26 21:14:52  jay
Added Time classes

Revision 1.3  2005/02/28 20:34:22  jay
Additions and fixes for Uptime Overlord v.1.0 release

Revision 1.2  2005/01/19 10:19:58  jay
Added MakeTime() to construct a time from days, months, etc.

Revision 1.1  2005/01/09 12:34:30  jay
New Time class - but calling the file Time.h causes problems.

*/

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

