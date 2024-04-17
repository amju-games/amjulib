// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <Timer.h>
#include "GuiDecAnimation.h"

namespace Amju
{
const char* GuiDecAnimation::NAME = "animation";

bool GuiDecAnimation::Save(File* f)
{
  if (!SaveTypeAndName(f))
  {
    return false;
  }
  if (!Animator::Save(f))
  {
    return false;
  }
  return GetChild()->Save(f);
}

bool GuiDecAnimation::Load(File* f)
{
  // Animations need names so we can get to them in code
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected animation name");
    return false;
  }

  if (!Animator::Load(f))
  {
    return false;
  }

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
  Animator::CalcUpdate(dt);

  if (Animator::DidReset())
  {
    OnResetAnimation();
  }

  // Animate descendants
  GetChild()->Animate(m_reverse ? 1.f - m_value : m_value);

  // Update descendants
  GetChild()->Update();
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

  OnResetAnimation();
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

