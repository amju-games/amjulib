// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include <Animator.h>
#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecAnimation *
// (Base) class for a Decorator which animates some property of its child.
// The default behaviour is to call Animate() on the child, passing in the 
//  animation value, which changes over time, between 0..1. So this class
//  can be used as is, or can be subclassed.
class GuiDecAnimation : public GuiDecorator, public Animator
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiDecAnimation* Clone() override { return new GuiDecAnimation(*this); }

  // Subclasses should call this to update m_time and m_value.
  // By default, m_value is used to set the animated property.
  virtual void Update() override;

  // Subclasses should call this to load animation time, loop type, etc.
  virtual bool Load(File*) override;
  virtual bool Save(File*) override;

  // Store animValue as time multiplier. I.e. the input to this object affects
  //  how fast this animation progresses. This can be used to make delays, and
  //  could do more complicated things.
  virtual void Animate(float animValue) override;

  // Reset time to zero. Pass message down to descendants.
  virtual void ResetAnimation() override;

  // Made redundant as we can call Animate(0) to pause, but hey
  void SetIsPaused(bool isPaused);
  bool IsPaused() const;

protected:

  bool m_isPaused = false;

};
}
