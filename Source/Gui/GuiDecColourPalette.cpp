// * Amjulib *
// (c) Copyright 2000-2026 Juliet Colman

#include <LoadPng.h>
#include "GuiDecColourPalette.h"

namespace Amju
{
const char* GuiDecColourPalette::NAME;

std::vector<Colour> RgbaToColourVec(
  std::vector<unsigned char>::iterator first,
  std::vector<unsigned char>::iterator last)
{
  std::vector<Colour> vec;
  while (first != last)
  {
    Colour c(*first++, *first++, *first++, *first++);
    vec.push_back(c);
  }
  return vec;
} 

bool GuiDecColourPalette::Load(File* f)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected colour palette image filename");
    return false;
  }

  Strings strs = Split(s, ',');
  Assert(!strs.empty());

  ColourMode::SetModeFromStrings(strs); // ColourMode mixin base class

  m_paletteFilename = strs[0];

  // Load the texture and convert its colours along the top row
  //  into a vector.
  int w = 0;
  int h = 0;
std::cout << "GuiDecColourPalette: loading palette: " << m_paletteFilename << "\n";

  std::vector<unsigned char> data;
  if (!LoadPng(m_paletteFilename, data, w, h))
  {
    f->ReportError("Failed to load palette: " + m_paletteFilename);
    return false;
  }
  if (w == 0 || h == 0)
  {
    f->ReportError("Bad image size? " + m_paletteFilename);
    return false;
  }
  if (h > 1)
  {
    f->ReportError("Only 0th row will be sampled: " + m_paletteFilename);
  }
  auto end = data.begin();
  std::advance(end, w * 4);
  // Convert range of raw RGBA bytes to vec of Colours
std::cout << "Convert to vec of colours....\n";

  m_palette = RgbaToColourVec(data.begin(), end);
std::cout << "OK! Converted to vec of colours.\n";
  return true;
}

bool GuiDecColourPalette::Save(File* f)
{
  if (!f->Write(GetTypeName()))
  {
    return false;
  }
  if (!f->Write(m_paletteFilename + ColourMode::GetStringFromMode()))
  {
    return false;
  }
  return GetChild()->Save(f);
}

void GuiDecColourPalette::Draw()
{
  PushColour();
  ColourMode::DrawColour(m_colour);
  GuiDecorator::Draw();
  PopColour();
}

void GuiDecColourPalette::Animate(float animValue)
{
  // Sample texure along u-axis 
  int size = static_cast<int>(m_palette.size());
  if (size > 0)
  {
    float fsize = static_cast<float>(size);
    int index = std::clamp(static_cast<int>(animValue * fsize + .5f), 0, size - 1);
    m_colour = m_palette[index];
  }
  GetChild()->Animate(animValue);
}
}

