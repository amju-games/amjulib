// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <Timer.h>
#include "GuiDecAnimation.h"

namespace Amju
{
  const char* GuiDecAnimation::NAME = "animation";

GuiDecAnimation::LoopType GuiDecAnimation::GetLoopTypeFromString(const std::string& s)
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

GuiDecAnimation::EaseType GuiDecAnimation::GetEaseTypeFromString(const std::string& cs, bool& reverse)
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
    { "delay", EaseType::EASE_TYPE_STEP },
    { "zero", EaseType::EASE_TYPE_ZERO },
    { "one", EaseType::EASE_TYPE_ONE },
  };
  auto it = EASE_TYPES.find(s);
  Assert(it != EASE_TYPES.end());
  return it->second;
}

void GuiDecAnimation::SetLoopType(LoopType loopType)
{
  m_loopType = loopType;
}

void GuiDecAnimation::SetEaseType(EaseType easeType)
{
  m_easeType = easeType;
}

void GuiDecAnimation::SetCycleTime(float cycleTime)
{
  m_cycleTime = cycleTime;
}

bool GuiDecAnimation::Load(File* f)
{
  // Animations need names so we can get to them in code
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected animation name");
    return false;
  }

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

  if (!GuiDecorator::Load(f))
  {
    return false;
  }

  return true;
}

void GuiDecAnimation::Update()
{
  if (!IsVisible())
  {
    return;
  }

  if (IsPaused())
  {
    return;
  }

  // Update current time
  float dt = TheTimer::Instance()->GetDt();
  CalcUpdate(dt);

  // Animate descendants
  m_children[0]->Animate(m_reverse ? 1.f - m_value : m_value);

  // Update descendants
  m_children[0]->Update();
}

void GuiDecAnimation::CalcUpdate(float dt)
{
  m_time += dt * m_timeMultiplier;

  switch (m_easeType)
  {
  case EaseType::EASE_TYPE_ZERO:
    m_value = 0;
    break;
  case EaseType::EASE_TYPE_ONE:
    m_value = 1;
    break;
  case EaseType::EASE_TYPE_LINEAR:
    m_value = m_time / m_cycleTime;
    break;
  case EaseType::EASE_TYPE_STEP:
    m_value = (m_time > m_cycleTime) ? 1.f : 0.f;
    break;
  }

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
      m_time -= m_cycleTime;
      m_value -= 1.0f;
      break;
    case LoopType::LOOP_TYPE_MIRROR_REPEAT:
      if (m_time > 2.0f * m_cycleTime)
      {
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

void GuiDecAnimation::Animate(float animValue)
{
  // This means the parent animation affects how fast this animation progresses.
  m_timeMultiplier = animValue;

  // We don't call Animate on our child here, because we do it in Update(),
  //  with our animated value.
}

void GuiDecAnimation::ResetAnimation()
{
  m_time = 0;
  GuiComposite::ResetAnimation();
}

void GuiDecAnimation::SetIsPaused(bool isPaused)
{
  m_isPaused = isPaused;
}

bool GuiDecAnimation::IsPaused() const
{
  return m_isPaused;
}
}

