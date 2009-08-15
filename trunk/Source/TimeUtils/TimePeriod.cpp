/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: TimePeriod.cpp,v $
Revision 1.4  2008/05/18 16:04:31  jay
MSVC update

Revision 1.3  2008/05/08 10:58:07  jay
New memory management code

Revision 1.2  2006/01/16 14:08:52  jay
Add Time classes to trunk

Revision 1.1.2.2  2005/08/31 22:44:56  Administrator
MSVC compile fixes

Revision 1.1.2.1  2005/08/26 21:14:52  jay
Added Time classes

Revision 1.4  2005/02/28 20:34:23  jay
Additions and fixes for Uptime Overlord v.1.0 release

Revision 1.3  2005/01/19 10:40:08  jay
Fixed bug in ToString() where days were wrong.

Revision 1.2  2005/01/09 12:37:53  jay
Additions to make class more useable/consistent

Revision 1.1  2005/01/03 11:08:55  jay
Added to Uptime Overlord project

*/

#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include "AmjuFirst.h"
#include "TimePeriod.h"
#include "StringUtils.h"
#include "SchAssert.h"
#include "AmjuFinal.h"

namespace Amju
{
TimePeriod::TimePeriod(unsigned int s) : m_secs(s)
{
  AMJU_CALL_STACK;

}

unsigned int TimePeriod::ToSeconds() const
{
  return m_secs;
}

TimePeriod& TimePeriod::operator+=(const TimePeriod& rhs)
{
  AMJU_CALL_STACK;

  m_secs += rhs.m_secs;
  return *this;
}

bool TimePeriod::operator<(const TimePeriod& rhs) const
{
  AMJU_CALL_STACK;

  return m_secs < rhs.m_secs;
}

bool TimePeriod::operator==(const TimePeriod& rhs) const
{
  AMJU_CALL_STACK;

  return m_secs == rhs.m_secs;
}

bool TimePeriod::operator>(const TimePeriod& rhs) const
{
  AMJU_CALL_STACK;

  return m_secs > rhs.m_secs;
}

std::string TimePeriod::ToString() const
{
  if (m_secs == 0)
  {
    return "0 secs";
  }

  int partsToShow = 2;
  int partsShown = 0;

  std::string s;
  unsigned int secs = m_secs;

  // TODO Different granularity depending on magnitude of m_secs.

  while (secs > 0 && partsShown < partsToShow)
  {
    if (secs < 60)
    {
      partsShown++;
   
      s += Amju::ToString(secs);
      s += " sec";
      if (secs != 1)
      {
        s += "s";
      }
      secs = 0; // to finish
    }
    else if (secs < (60 * 60)) // i.e. less than an hour
    {
      partsShown++;
   
      unsigned int mins = secs / 60;
      secs -= mins * 60;
      Assert(secs < 60);
      s += Amju::ToString(mins);
      s += " min";
      if (mins != 1)
      {
        s += "s";
      }
      if (secs > 0)
      {
        s += " "; 
      }
    }
    else if (secs < (60 * 60 * 24)) // i.e. less than a day
    {
      partsShown++;
   
      unsigned int hours = secs / 3600;
      secs -= hours * 3600;
      // Round to whole no of minutes ?
      s += Amju::ToString(hours);
      s += " hour";
      if (hours != 1)
      {
        s += "s";
      }
      if (secs > 0)
      {
        s += " ";
      }
    }
    else if (secs < (60 * 60 * 24 * 7)) // i.e. less than one week
    {
      partsShown++;
   
      unsigned int days = secs / (24 * 3600);
      secs -= days * 24 * 3600;
      s += Amju::ToString(days);
      s += " day";
      if (days != 1)
      {
        s += "s";
      }
      if (secs > 0)
      {
        s += " ";
      }
    }
    else if (secs < (60 * 60 * 24 * 7 * 52)) // less than one year
    {
      partsShown++;
   
      unsigned int weeks = secs / (24 * 3600 * 7);
      secs -= weeks * 24 * 3600 * 7;
      s += Amju::ToString(weeks);
      s += " week";
      if (weeks != 1)
      {
        s += "s";
      }
      if (secs > 0)
      {
        s += " ";
      }
    }
    else if (secs >= (60 * 60 * 24 * 7 * 52))
    {
      partsShown++;
   
      unsigned int years = secs / (24 * 3600 * 7 * 52);
      secs -= years * 24 * 3600 * 7 * 52;
      s += Amju::ToString(years);
      s += " year";
      if (years != 1)
      {
        s += "s";
      }
      if (secs > 0)
      {
        s += " ";
      }
    }
    else
    {
      partsShown++;
   
      s += Amju::ToString(secs);
      s += "secs (unexpected)";
      secs = 0;
    }
  
  } // while

  return s;
}

}


