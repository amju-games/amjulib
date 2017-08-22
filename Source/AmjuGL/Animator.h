// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include <string>

namespace Amju
{
class File;

// * Animator *
// Mixin class which calculates animation curves.
// Can be used for GUI tweening, and animating scene nodes.
class Animator
{
public:
  virtual ~Animator() = default;

  bool Load(File*);

  void CalcUpdate(float dt);

  // Returns true if the last call to CalcUpdate caused the animation to reset
  bool DidReset() const;

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
    EASE_TYPE_IN_OUT,
    EASE_TYPE_IN_OUT_ELASTIC,
  };

  void SetIsReversed(bool reverse);
  bool IsReversed() const;

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

  // Scales how fast time elapses; 0 means paused, 1 means running at normal speed.
  // Set by parent calling the Animate function, so we can chain animations to get
  //  delays etc.
  float m_timeMultiplier = 1.0f;

  bool m_reverse = false;

  bool m_didReset = false;
};
}

