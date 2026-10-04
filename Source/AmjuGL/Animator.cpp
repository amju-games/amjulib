// * Amjulib *
// (c) Copyright 2000-2017 Juliet Colman

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include <AmjuAssert.h>
#include "Animator.h"
#include "File.h"

namespace Amju
{
Animator::Animator()
{
  SetEaseName("linear");
}

static float EaseInOutElastic(float t, float period = 0.45f, float power = 8.0f)
{
  if (t <= 0.0f || t >= 1.0f)
  {
    return std::clamp(t, 0.0f, 1.0f);
  }

  const float s = period / 4.0f;
  const float c = (2.0f * static_cast<float>(M_PI)) / period;
  const float tScaled = 2.0f * t - 1.0f;

  if (t < 0.5f)
  {
    return -(powf(2.0f, power * tScaled) * sinf((tScaled - s) * c)) / 2.0f;
  }
  else
  {
    return (powf(2.0f, -power * tScaled) * sinf((tScaled - s) * c)) / 2.0f + 1.0f;
  }
}

Animator::EaseFactory& Animator::GetEaseFactory()
{
  static EaseFactory factory;
  static bool initialized = false;
  if (!initialized)
  {
    initialized = true;
    factory.Add("linear", [](float t) { return t; });
    factory.Add("zero", [](float) { return 0.0f; });
    factory.Add("one", [](float) { return 1.0f; });
    factory.Add("set", nullptr); // nullptr signifies 'SET' behavior (no overwrite)
    factory.Add("sine", [](float t) 
    { 
      return (sinf(t * 2.f * static_cast<float>(M_PI)) + 1.f) * .5f; 
    });
    factory.Add("step", [](float t) { return t >= 1.0f ? 1.0f : 0.0f; });
    factory.Add("ease-in-out", [](float t) 
    { 
      return 0.5f * (1.f - cosf(t * static_cast<float>(M_PI))); 
    });

    factory.Add("ease-in-out-elastic", [](float t)
    {
      return EaseInOutElastic(t, 0.45f, 8.0f); // Adjust 8.0f lower for a gentler S-curve
    });
  }
  return factory;
}

bool Animator::Save(File* f)
{
  static const std::map<LoopType, std::string> LOOP_TYPES =
  {
    { LoopType::LOOP_TYPE_CONST, "const" },
    { LoopType::LOOP_TYPE_ONE_SHOT, "one-shot" },
    { LoopType::LOOP_TYPE_REPEAT, "repeat" },
    { LoopType::LOOP_TYPE_MIRROR_REPEAT, "mirror-repeat" },
  };
  
  if (!f->Write(LOOP_TYPES.at(m_loopType)))
  {
    return false;
  }
  if (m_loopType != LoopType::LOOP_TYPE_CONST)
  {
    f->WriteComment("// cycle time");
    f->WriteFloat(m_cycleTime);
  }
  f->WriteComment("// easing function");
  
  std::string s = m_reverse ? "reverse-" : "";
  s += m_easeName;
  return f->Write(s);
}

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
  
  std::string s(line);
  m_reverse = false;
  static const std::string REVERSE = "reverse-";

  if (s.compare(0, REVERSE.length(), REVERSE) == 0)
  {
    s = s.substr(REVERSE.length());
    m_reverse = true;
  }

  SetEaseName(s);
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
  return LOOP_TYPES.at(s);
}

void Animator::SetEaseName(const std::string& name)
{
  m_easeName = name;
  m_easeFunc = GetEaseFactory().Create(name);
}

const std::string& Animator::GetEaseName() const
{
  return m_easeName;
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

void Animator::SetCycleTime(float cycleTime)
{
  m_cycleTime = cycleTime;
}

float Animator::GetCycleTime() const
{
  return m_cycleTime;
}

float Animator::GetAnimTimeSeconds() const
{
  return m_time;
}

void Animator::SetAnimTimeSeconds(float seconds)
{
  m_time = seconds;
}

void Animator::SetValue(float value)
{
  m_value = std::clamp(value, 0.f, 1.f);
}

bool Animator::DidReset() const
{
  return m_didReset;
}

void Animator::SetOnCompleteCallback(AnimCallback cb)
{
  m_onComplete = cb;
}

void Animator::CalcUpdate(float dt)
{
  m_time += dt * m_timeMultiplier;
  m_didReset = false;

  // 1. Handle time wrapping based on loop type BEFORE calculating ease
  if (m_time > m_cycleTime)
  {
    switch (m_loopType)
    {
    case LoopType::LOOP_TYPE_CONST:
      break;

    case LoopType::LOOP_TYPE_ONE_SHOT:
      m_time = m_cycleTime; // clamp time so t never exceeds 1.0
      if (m_onComplete)
      {
        m_onComplete(this);
        m_onComplete = nullptr; // Ensure it only fires once
      }
      break;

    case LoopType::LOOP_TYPE_REPEAT:
      while (m_time >= m_cycleTime && m_cycleTime > 0.0f)
      {
        m_time -= m_cycleTime;
        m_didReset = true;
      }
      break;

    case LoopType::LOOP_TYPE_MIRROR_REPEAT:
      while (m_time >= 2.0f * m_cycleTime && m_cycleTime > 0.0f)
      {
        m_time -= 2.0f * m_cycleTime;
        m_didReset = true;
      }
      break;
    }
  }

  // 2. Calculate normalized t (always strictly 0.0 to 1.0)
  float t = 0.0f;
  if (m_cycleTime > 0.0f)
  {
    if (m_loopType == LoopType::LOOP_TYPE_MIRROR_REPEAT && m_time > m_cycleTime)
    {
      // We are in the reversing phase of the mirror loop: t goes from 1 down to 0
      t = (2.0f * m_cycleTime - m_time) / m_cycleTime;
    }
    else
    {
      // Forward phase
      t = m_time / m_cycleTime;
    }

    t = std::clamp(t, 0.0f, 1.0f);
  }

  // 3. Apply easing safely
  if (m_easeFunc)
  {
    m_value = m_easeFunc(t);
  }
}
}

