// * Amjulib *
// (c) Copyright 2000-2026 Juliet Colman

#pragma once

#include <Colour.h>
#include <StringUtils.h>

namespace Amju
{
// * ColourMode *
// Mixin class for colour decorators
class ColourMode 
{
protected:
  // Call from Load() in client class, when parsing strings.
  // Looks for "add" and "reset" strings, and sets m_mode accordingly.
  // Removes these strings on return so the rest of the strings can be
  //  more easily parsed. (i.e. "add" or "reset" strings could go at 
  //  any position in the input.)
  void SetModeFromStrings(Strings& strs);

  // Returns a string to append to other colour info (i.e. it will begin
  //  with ',', so is not a standalone line).
  // Use this in save as the counterpart to SetModeFromStrings.
  std::string GetStringFromMode() const;

  // Call from client Draw function, to use the final colour according
  //  to the colour mode.
  void DrawColour(const Colour& c) const;

protected:
  // Mode: what we do with the colour passed into DrawColour().
  // If DEC_COLOUR_MULT, we multiply c by the prevailing ancestor colour.
  // This is the default mode.
  // If DEC_COLOUR_RESET, we set current colour to c, ignoring ancestor colour. 
  // This gives a way to reset the colour in a gui tree.
  // To set this mode, include "reset" in the colour string.
  // If DEC_COLOUR_ADD, we add c to the prevailing ancestor colour.
  // To set this mode, include "add" in the colour string.
  enum Mode
  {
    DEC_COLOUR_MULT,
    DEC_COLOUR_RESET,
    DEC_COLOUR_ADD
  };
  Mode m_mode = DEC_COLOUR_MULT;
};
}

