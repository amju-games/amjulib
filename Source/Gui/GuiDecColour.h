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
  Colour GetColour() const override;

protected:
  Colour m_secondColour;
  Colour m_interpolatedColour;
  // Mode: what we do with the interpolated colour.
  // If DEC_COLOUR_MULT, we multiply the interpolated colour by the prevailing ancestor colour.
  // This is the default mode.
  // If DEC_COLOUR_RESET, we set the colour, ignoring ancestor colour. This gives a way
  //  to reset the colour in a gui tree, as requested below.
  // To set this mode, include "reset" in the colour string.
  // If DEC_COLOUR_ADD, we add the interpolated colour to the prevailing ancestor colour.
  // To set this mode, include "add" in the colour string.
  enum Mode
  {
    DEC_COLOUR_MULT,
    DEC_COLOUR_RESET,
    DEC_COLOUR_ADD
  };
  Mode m_mode = DEC_COLOUR_MULT;
};

// TODO also a colour SETTER, so we can reset the current colour in the tree.
// This would be useful if e.g. you fade out the whole tree but have some
//  elements you don't want to fade.
}
