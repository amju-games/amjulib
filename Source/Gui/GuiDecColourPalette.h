// * Amjulib *
// (c) Copyright 2000-2026 Juliet Colman

#pragma once

#include "GuiDecColour.h"

namespace Amju
{
// Loads a palette as a 1D texture. The output colour is the (u, v) texel
//  where u is the animation value passed into Animate(), and v==0.
// No interpolation between adjacent colours, i.e. like 'nearest' filtering.
// The output colour is used according to the Colour Mode.
class GuiDecColourPalette : public GuiDecorator, public ColourMode
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiDecColourPalette* Clone() override { return new GuiDecColourPalette(*this); }

  bool Load(File*) override;
  bool Save(File*) override;
  void Draw() override;
  void Animate(float animValue) override;

protected:
  // 1D (horizontal) texture of colours.
  std::vector<Colour> m_palette;
  std::string m_paletteFilename; // image filename for saving.
};
}

