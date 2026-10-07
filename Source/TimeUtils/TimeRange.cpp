// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include <AmjuFirst.h>
#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include "TimeRange.h"
#include "AmjuTime.h"
#include "AmjuAssert.h"
#include <AmjuFinal.h>

namespace Amju
{
TimeRange::TimeRange(unsigned int start, unsigned int end)
 : m_start(start), m_end(end)
{
  AMJU_CALL_STACK;

  if (m_start > m_end)
  {
    unsigned int t = m_start;
    m_start = m_end;
    m_end = t;
  }
}

TimeRange::TimeRange(const Time& t1, const Time& t2)
{
  AMJU_CALL_STACK;

  m_start = t1.m_secs;
  m_end = t2.m_secs;
  if (m_start > m_end)
  {
    unsigned int t = m_start;
    m_start = m_end;
    m_end = t;
  }
}

Time TimeRange::GetStart() const
{
  AMJU_CALL_STACK;

  return m_start;
}

Time TimeRange::GetEnd() const
{
  AMJU_CALL_STACK;

  return m_end;
}

void TimeRange::SetStart(const Time& t)
{
  AMJU_CALL_STACK;

  m_start = t.m_secs;
  Assert(m_start <= m_end);
}

void TimeRange::SetEnd(const Time& t)
{
  AMJU_CALL_STACK;

  m_end = t.m_secs;
  Assert(m_start <= m_end);
}

std::string TimeRange::ToString() const
{
  if (m_start == m_end)
  {
    return Time(m_start).ToString();
  }

  std::string s = "From ";
  s += Time(m_start).ToString();
  s += " to ";
  s += Time(m_end).ToString();

  return s; 
}

bool TimeRange::IsInRange(unsigned int t) const
{
  AMJU_CALL_STACK;

  return (t >= m_start && t < m_end);
}

bool TimeRange::IsEarlierThan(const TimeRange& rhs) const
{
  AMJU_CALL_STACK;

  // NB If the TimeRanges "touch" but do not overlap, then this range is
  // considered to be "earlier" than rhs. I.e. it's like 'less than or equal to'.
  return (m_end <= rhs.m_start);
}

TimePeriod TimeRange::GetPeriod() const
{
  AMJU_CALL_STACK;

  Assert(m_end >= m_start);
  unsigned int d = m_end - m_start;
  return TimePeriod(d);
}
}

