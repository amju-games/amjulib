// * Amjulib *
// (c) Copyright 2000-2017 Juliet Colman

#pragma once

#include <Colour.h>
#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecColour *
// Decorator which modulates the current colour for descendants. 
// The colour can be animated, by interpolating between two colours.
class GuiDecColour : public GuiDecorator
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiDecColour* Clone() override { return new GuiDecColour(*this); }

  bool Load(File*) override;
  bool Save(File*) override;
  void Draw() override;
  void Animate(float animValue) override;

  // Second colour: when we animate we interpolate between the two colours.
  void SetSecondColour(const Colour&);
  Colour GetColour() const;

protected:
  Colour m_secondColour;
  Colour m_interpolatedColour;
};

// TODO also a colour SETTER, so we can reset the current colour in the tree.
// This would be useful if e.g. you fade out the whole tree but have some
//  elements you don't want to fade.
}