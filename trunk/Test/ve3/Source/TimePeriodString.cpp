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
    return past ? "less than a minute ago" : "less than a minute";
  }
  else if (sec < 5 * ONE_MINUTE)
  {
    return past ? "less than 5 minutes ago" : "less than 5 minutes";
  }
  else if (sec < 10 * ONE_MINUTE)
  {
    return past ? "less than 10 minutes ago" : "less than 10 minutes";
  }
  else if (sec < 30 * ONE_MINUTE)
  {
    return past ? "less than half an hour ago" : "less than half an hour";
  }
  else if (sec < ONE_HOUR)
  {
    return past ? "less than an hour ago" : "less than an hour";
  }
  else if (sec < ONE_DAY_IN_SECONDS)
  {
    return past ? "today" : "less than one day";
  }
  else if (sec < 2 * ONE_DAY_IN_SECONDS)
  {
    return past ? "yesterday" : "less than 2 days";
  }
  else if (sec < ONE_WEEK_IN_SECONDS)
  {
    return past ? "less than a week ago" : "less than a week";
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
