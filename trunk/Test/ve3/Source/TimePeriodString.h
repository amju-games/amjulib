#pragma once

#include <TimePeriod.h>

namespace Amju
{
// Convert time period to informal string like "Less than a minute".
// If pastTense, strings are in form of "5 minutes ago" and "yesterday", 
//  rather than "5 minutes" and "1 day".
  
// TODO Localise
std::string InformalTimePeriodString(
  TimePeriod period, 
  bool capitaliseFirstLetter, 
  bool pastTense);
}
