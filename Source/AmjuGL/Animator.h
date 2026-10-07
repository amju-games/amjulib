// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#pragma once

#include <functional>
#include <string>
#include "FunctionFactory.h"

namespace Amju
{
class Animator;
class File;

// * AnimCallback *
// Function type for callback when anim completes, etc.
using AnimCallback = std::function<void(Animator*)>;

// * Animator *
// Mixin class which calculates animation curves.
// Can be used for GUI tweening, and animating scene nodes.
class Animator
{
public:
  using EaseFunc = std::function<float(float)>;
  using EaseFactory = FunctionFactory<EaseFunc>;

  Animator();
  virtual ~Animator() = default;

  virtual bool Load(File*);
  virtual bool Save(File*);

  void CalcUpdate(float dt);

  // Returns true if the last call to CalcUpdate caused the animation to reset
  bool DidReset() const;

  // Set a callback function which will be called when the anim completes.
  // Only applies to one-shot anims.
  void SetOnCompleteCallback(AnimCallback cb);

  enum class LoopType
  {
    LOOP_TYPE_CONST,
    LOOP_TYPE_ONE_SHOT,
    LOOP_TYPE_REPEAT,
    LOOP_TYPE_MIRROR_REPEAT
  };

  void SetLoopType(LoopType loopType);

  void SetIsReversed(bool reverse);
  bool IsReversed() const;

  // Set the easing function by name. Will fetch from the global EaseFactory.
  void SetEaseName(const std::string& easeName);
  const std::string& GetEaseName() const;

  // Access the global factory to register new easing functions at runtime
  static EaseFactory& GetEaseFactory();

  // For repeating anims, get the time for a full cycle of this animation.
  // For one shot anims, it's not a cycle, it's the total time before
  //  the anim finishes.
  // For constant value anims, returns last set cycle time.
  float GetCycleTime() const;

  void SetCycleTime(float cycleTime);

  // Get the current anim time value
  float GetAnimTimeSeconds() const;

  // Set the anim time value
  void SetAnimTimeSeconds(float seconds);

  // Set value, 0..1. This will be overwritten in the next update,
  //  except for const loop types. 
  // The use case is for const anims that control, say, a chooser 
  //  decorator, where the set value won't be overwritten and can
  //  be used by the thing we are animating.
  void SetValue(float value);

  // Conceptually protected but public for testing:
  static LoopType GetLoopTypeFromString(const std::string& s);

protected:

  LoopType m_loopType = LoopType::LOOP_TYPE_ONE_SHOT;

  std::string m_easeName;
  EaseFunc m_easeFunc;

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

  // Call when anim completes
  AnimCallback m_onComplete;
};
}

