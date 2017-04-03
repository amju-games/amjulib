// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecAnimation *
// Base class for a Decorator which animates some property of its child.
class GuiDecAnimation: public GuiDecorator
{
public:
  // Subclasses should call this to update m_time and m_value.
  // m_value can be used to set the animated property.
  virtual void Update() override;

  // Subclasses should call this to load animation time, loop type, etc.
  virtual bool Load(File*) override;

  void SetIsPaused(bool isPaused);
  bool IsPaused() const;

  // Called from Update, and can be called separately for testing
  void CalcUpdate(float dt);

  enum class LoopType
  {
    LOOP_TYPE_ONE_SHOT,
    LOOP_TYPE_REPEAT,
    LOOP_TYPE_MIRROR_REPEAT
  };

  void SetLoopType(LoopType loopType);

  void SetCycleTime(float cycleTime);

protected:

  // TODO curve type: linear, ease in/out etc

  static LoopType GetLoopTypeFromString(const std::string& s);

  LoopType m_loopType = LoopType::LOOP_TYPE_ONE_SHOT;

  // Current elapsed time; does not update if element is invisible
  //  or paused
  float m_time = 0;

  // Duration of the animation, i.e. the time it takes m_value to go from 0 to 1
  float m_cycleTime = 1.0f;

  // Varies between 0..1, according to loop type and elapsed time.
  float m_value = 0;

  bool m_isPaused = false;
};
}
