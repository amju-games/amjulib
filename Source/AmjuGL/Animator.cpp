// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <map>
#include <string>
#include <AmjuAssert.h>
#include <Animator.h>
#include <File.h>

namespace Amju
{
bool Animator::Load(File* f)
{
  std::string line;

  // Get loop type
  if (!f->GetDataLine(&line))
  {
    f->ReportError("Expected loop type");
    return false;
  }
  m_loopType = GetLoopTypeFromString(line);

  // Get cycle time, except not for a const anim
  if (m_loopType != LoopType::LOOP_TYPE_CONST)
  {
    if (!f->GetFloat(&m_cycleTime))
    {
      f->ReportError("Expected animation cycle time");
      return false;
    }
  }

  // Get easing function
  if (!f->GetDataLine(&line))
  {
    f->ReportError("Expected easing function");
    return false;
  }
  m_easeType = GetEaseTypeFromString(line, m_reverse);

  return true;
}

Animator::LoopType Animator::GetLoopTypeFromString(const std::string& s)
{
  static const std::map<std::string, LoopType> LOOP_TYPES =
  {
    { "const", LoopType::LOOP_TYPE_CONST },
    { "one-shot", LoopType::LOOP_TYPE_ONE_SHOT },
    { "repeat", LoopType::LOOP_TYPE_REPEAT },
    { "mirror-repeat", LoopType::LOOP_TYPE_MIRROR_REPEAT },
  };
  auto it = LOOP_TYPES.find(s);
  Assert(it != LOOP_TYPES.end());
  return it->second;
}

Animator::EaseType Animator::GetEaseTypeFromString(const std::string& cs, bool& reverse)
{
  std::string s(cs);
  // Check for "reverse-" at front of string
  reverse = false;
  static const std::string REVERSE = "reverse-";

  if (s.compare(0, REVERSE.length(), REVERSE) == 0)
  {
    s = s.substr(REVERSE.length());
    reverse = true;
  }

  static const std::map<std::string, EaseType> EASE_TYPES =
  {
    { "linear", EaseType::EASE_TYPE_LINEAR },
    { "step", EaseType::EASE_TYPE_STEP },
    { "zero", EaseType::EASE_TYPE_ZERO },
    { "one", EaseType::EASE_TYPE_ONE },
    { "ease-in-out", EaseType::EASE_TYPE_IN_OUT },
    { "ease-in-out-elastic", EaseType::EASE_TYPE_IN_OUT_ELASTIC },
  };
  auto it = EASE_TYPES.find(s);
  Assert(it != EASE_TYPES.end());
  return it->second;
}

void Animator::SetIsReversed(bool reverse)
{
  m_reverse = reverse;
}

bool Animator::IsReversed() const
{
  return m_reverse;
}

void Animator::SetLoopType(LoopType loopType)
{
  m_loopType = loopType;
}

void Animator::SetEaseType(EaseType easeType)
{
  m_easeType = easeType;
}

void Animator::SetCycleTime(float cycleTime)
{
  m_cycleTime = cycleTime;
}

static float EaseInOut(float t)
{
  // TODO I think this one would work with mirror repeat, TODO test that thought.
  // https://github.com/warrenm/AHEasing/blob/master/AHEasing/easing.c
  return 0.5f * (1.f - cosf((float)(t * M_PI)));

  // https://math.stackexchange.com/questions/121720/ease-in-out-function
  //  return (t * t) / (t * t +  (1.f - t) * (1.f - t));
}

static float EaseInOutElastic(float t)
{
  return t; // TODO
}

bool Animator::DidReset() const
{
  return m_didReset;
}

void Animator::CalcUpdate(float dt)
{
  m_time += dt * m_timeMultiplier;

  // Varies 0..1 
  float t = m_time / m_cycleTime;

  switch (m_easeType)
  {
  case EaseType::EASE_TYPE_ZERO:
    m_value = 0;
    break;
  case EaseType::EASE_TYPE_ONE:
    m_value = 1;
    break;
  case EaseType::EASE_TYPE_LINEAR:
    m_value = t;
    break;
  case EaseType::EASE_TYPE_STEP:
    m_value = (m_time > m_cycleTime) ? 1.f : 0.f;
    break;
  case EaseType::EASE_TYPE_IN_OUT:
    m_value = EaseInOut(t);
    break;
  case EaseType::EASE_TYPE_IN_OUT_ELASTIC:
    m_value = EaseInOutElastic(t);
    break;
  }

  m_didReset = false;
  if (m_time > m_cycleTime)
  {
    switch (m_loopType)
    {
    case LoopType::LOOP_TYPE_CONST:
      // Don't care, value doesn't depend on time
      break;
    case LoopType::LOOP_TYPE_ONE_SHOT:
      m_value = 1.0f;
      break;
    case LoopType::LOOP_TYPE_REPEAT:
      m_didReset = true;

      m_time -= m_cycleTime;
      m_value -= 1.0f;
      break;
    case LoopType::LOOP_TYPE_MIRROR_REPEAT:
      // Pobably only works with step and linear
      if (m_time > 2.0f * m_cycleTime)
      {
        m_didReset = true;

        m_time -= 2.0f * m_cycleTime;
        m_value -= 2.0f;
      }
      else
      {
        m_value = 2.0f - m_value;
      }
      break;
    }
  }
}
}