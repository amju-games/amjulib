// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecTranslate *
// Decorator which translates child. This can be animated: we can specify 2 positions, and
//  use an animation decorator to control interpolation between them.
// If two positions are given, separate them with semicolon.
class GuiDecTranslate : public GuiDecorator
{
public:
  static const char* NAME;

  virtual bool Load(File*) override;

  virtual void Animate(float animValue) override;

private:
  Vec2f m_positions[2];
};
}

