/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: TimePeriod.h,v $
Revision 1.2  2006/01/16 14:08:52  jay
Add Time classes to trunk

Revision 1.1.2.1  2005/08/26 21:14:52  jay
Added Time classes

Revision 1.3  2005/02/28 20:34:23  jay
Additions and fixes for Uptime Overlord v.1.0 release

Revision 1.2  2005/01/09 12:37:53  jay
Additions to make class more useable/consistent

Revision 1.1  2005/01/03 11:08:55  jay
Added to Uptime Overlord project

*/

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


