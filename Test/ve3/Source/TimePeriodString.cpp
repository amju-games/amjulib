#include <AmjuTime.h>
#include "TimePeriodString.h"

namespace Amju
{
std::string InformalTimePeriodStringNotCapitalised(
  TimePeriod period, 
  bool past)
{
  unsigned int sec = period.ToSeconds();
  if (sec < ONE_MINUTE)
  {
    return past ? "a minute ago" : "a minute";
  }
  else if (sec < ONE_HOUR)
  {
    return past ? "an hour ago" : "an hour";
  }
  else if (sec < ONE_DAY_IN_SECONDS)
  {
    return past ? "yesterday" : "one day";
  }
  else if (sec < ONE_WEEK_IN_SECONDS)
  {
    return past ? "a week ago" : "a week";
  }
  return past ? "over a week ago" : "more than a week";
}

std::string InformalTimePeriodString(
  TimePeriod period, 
  bool capitaliseFirstLetter, 
  bool pastTense)
{
  std::string s = InformalTimePeriodStringNotCapitalised(period, pastTense);
  if (capitaliseFirstLetter && !s.empty())
  {
    s[0] = toupper(s[0]);
  }
  return s;
}
}
