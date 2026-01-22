// * Amjulib *
// (c) Copyright 2000-2026 Juliet Colman

#pragma once

#include "GuiDecAnimation.h"

namespace Amju
{
// Chooses a value at random (0..1) at load time, and then always animates
//  descendants with that same value. Decorators can use this value for
//  random colours, transforms, etc.
class GuiDecAnimRandom : public GuiDecAnimation
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiDecAnimRandom* Clone() override { return new GuiDecAnimRandom(*this); }

  virtual void Update() override;

  // Subclasses should call this to load animation time, loop type, etc.
  virtual bool Load(File*) override;
  virtual bool Save(File*) override;

  // Store animValue as time multiplier. I.e. the input to this object affects
  //  how fast this animation progresses. This can be used to make delays, and
  //  could do more complicated things.
  virtual void Animate(float animValue) override;

protected:
  // random value between 0 and 1 chosen at load time; not loaded or saved.
  float m_random = 0; 
};
}

