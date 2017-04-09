// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecAnimation *
// (Base) class for a Decorator which animates some property of its child.
// The default behaviour is to call Animate() on the child, passing in the 
//  animation value, which changes over time, between 0..1. So this class
//  can be used as is, or can be subclassed.
class GuiDecAnimation: public GuiDecorator
{
public:
  static const char* NAME;

  // Subclasses should call this to update m_time and m_value.
  // By default, m_value is used to set the animated property.
  virtual void Update() override;

  // Subclasses should call this to load animation time, loop type, etc.
  virtual bool Load(File*) override;

  // Store animValue as time multiplier. I.e. the input to this object affects
  //  how fast this animation progresses. This can be used to make delays, and
  //  could do more complicated things.
  virtual void Animate(float animValue) override;

  // Reset time to zero. Pass message down to descendants.
  virtual void ResetAnimation() override;

  // Made redundant as we can call Animate(0) to pause, but hey
  void SetIsPaused(bool isPaused);
  bool IsPaused() const;

  // Called from Update, and can be called separately for testing.
  // dt is multiplied by m_timeMultiplier in this function.
  void CalcUpdate(float dt);

  enum class LoopType
  {
    LOOP_TYPE_CONST,
    LOOP_TYPE_ONE_SHOT,
    LOOP_TYPE_REPEAT,
    LOOP_TYPE_MIRROR_REPEAT
  };

  void SetLoopType(LoopType loopType);

  enum class EaseType
  {
    EASE_TYPE_ZERO,
    EASE_TYPE_ONE,
    EASE_TYPE_LINEAR,
    EASE_TYPE_STEP,
  };

  void SetEaseType(EaseType easeType);

  void SetCycleTime(float cycleTime);

// Conceptually protected but public for testing:

  static LoopType GetLoopTypeFromString(const std::string& s);

  static EaseType GetEaseTypeFromString(const std::string& s, bool& reverse);

protected:

  LoopType m_loopType = LoopType::LOOP_TYPE_ONE_SHOT;

  EaseType m_easeType = EaseType::EASE_TYPE_LINEAR;

  // Current elapsed time; does not update if element is invisible
  //  or paused
  float m_time = 0;

  // Duration of the animation, i.e. the time it takes m_value to go from 0 to 1
  float m_cycleTime = 1.0f;

  // Varies between 0..1, according to loop type and elapsed time.
  float m_value = 0;

  bool m_isPaused = false;

  // Scales how fast time elapses; 0 means paused, 1 means running at normal speed.
  // Set by parent calling the Animate function, so we can chain animations to get
  //  delays etc.
  float m_timeMultiplier = 1.0f;

  bool m_reverse = false;
};
}
