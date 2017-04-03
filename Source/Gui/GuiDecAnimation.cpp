// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <Timer.h>
#include "GuiDecAnimation.h"

namespace Amju
{
GuiDecAnimation::LoopType GuiDecAnimation::GetLoopTypeFromString(const std::string& s)
{
  static const std::map<std::string, LoopType> LOOP_TYPES = 
  {
    { "one-shot", LoopType::LOOP_TYPE_ONE_SHOT },
    { "repeat", LoopType::LOOP_TYPE_REPEAT },
    { "mirror-repeat", LoopType::LOOP_TYPE_MIRROR_REPEAT },
  };
  auto it = LOOP_TYPES.find(s);
  Assert(it != LOOP_TYPES.end());
  return it->second;
}

void GuiDecAnimation::SetLoopType(LoopType loopType)
{
  m_loopType = loopType;
}

void GuiDecAnimation::SetCycleTime(float cycleTime)
{
  m_cycleTime = cycleTime;
}

bool GuiDecAnimation::Load(File* f)
{
  // Get cycle time
  if (!f->GetFloat(&m_cycleTime))
  {
    f->ReportError("Expected animation cycle time");
    return false;
  }

  // Get animation type
  std::string line;
  if (!f->GetDataLine(&line))
  {
    f->ReportError("Expected animation type");
    return false;
  }
  m_loopType = GetLoopTypeFromString(line);

  // Initially paused?
  // Use another decorator around this one to set that kind of state???

  return GuiDecorator::Load(f);
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
}

void GuiDecAnimation::CalcUpdate(float dt)
{
  m_time += dt;

  // Linear curve TODO other types
  m_value = m_time / m_cycleTime;

  if (m_time > m_cycleTime)
  {
    switch (m_loopType)
    {
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
        m_value -= 1.0f;
      }
      else
      {
        m_value = 1.0f - m_value;
      }
      break;
    }
  }
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

