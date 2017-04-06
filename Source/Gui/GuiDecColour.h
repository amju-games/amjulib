// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include <Colour.h>
#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecColour *
// Decorator which sets colour for descendants. The colour can be animated,
//  by interpolating between two colours.
class GuiDecColour : public GuiDecorator
{
public:
  static const char* NAME;

  virtual bool Load(File*) override;
  virtual void Draw() override;
  virtual void Animate(float animValue) override;

  void SetColour(const Colour&, int zeroOrOne);

protected:
  Colour m_colour[2];
  Colour m_interpolatedColour;
};
}