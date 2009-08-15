/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: TimeRange.h,v $
Revision 1.2  2006/01/16 14:08:52  jay
Add Time classes to trunk

Revision 1.1.2.1  2005/08/26 21:14:52  jay
Added Time classes

Revision 1.2  2005/01/09 12:37:03  jay
Additions to make class more useable

Revision 1.1  2005/01/03 11:08:55  jay
Added to Uptime Overlord project

*/

#ifndef TIME_RANGE_H_INCLUDED
#define TIME_RANGE_H_INCLUDED

#include <string>
#include "TimePeriod.h"
#include "AmjuTime.h"

namespace Amju
{
// Specifies two absolute dates+times.
// The first timestamp should be before, or equal to the second one.
// If the timestamps are equal, the range is zero seconds.
class TimeRange
{
public:
  // Construct from two unix timestamps in secs.
  // start should be earlier or same as end.
  // If this is not the case, the two are swapped.
  TimeRange(unsigned int start, unsigned int end);

  // Construct from two Times. As above, the end must always be equal or later
  // to the start, or the two are swapped.
  TimeRange(const Time&, const Time&); 

  void SetStart(const Time&);
  void SetEnd(const Time&);

  Time GetStart() const;
  Time GetEnd() const;

  std::string ToString() const;
  bool IsInRange(unsigned int) const;
  // The period is the difference between the timestamps.
  TimePeriod GetPeriod() const;

  // Compare Time Ranges
  // -------------------
  // Return true if this TimeRange starts and finishes before the argument
  // TimeRange starts.
  bool IsEarlierThan(const TimeRange&) const;

  // TODO more comparison functions

protected:
  unsigned int m_start, m_end;
};
}

#endif

