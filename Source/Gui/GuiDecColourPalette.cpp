// * Amjulib *
// (c) Copyright 2000-2026 Juliet Colman

#include <LoadPng.h>
#include "GuiDecColourPalette.h"

namespace Amju
{
void ReportError(const std::string& str);

const char* GuiDecColourPalette::NAME = "colour-palette";

static Colour FromBytes(unsigned char* bytes)
{
  return Colour(
    static_cast<float>(bytes[0]) / 255.0f,
    static_cast<float>(bytes[1]) / 255.0f,
    static_cast<float>(bytes[2]) / 255.0f,
    static_cast<float>(bytes[3]) / 255.0f);
}

static std::vector<Colour> RgbaToColourVec(
  std::vector<unsigned char>::iterator first,
  std::vector<unsigned char>::iterator last)
{
  std::vector<Colour> vec;
  while (first != last)
  {
    Colour c = FromBytes(&(*first));
    first += 4;
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

  auto optPalette = ImageToColourVec(m_paletteFilename);
  if (!optPalette) return false;

  m_palette = *optPalette;

  return GuiDecorator::LoadOneChild(f);
}

std::optional<std::vector<Colour>> ImageToColourVec(const std::string& imageFilename)
{
  // Load image and convert its colours along the top row
  //  into a vector.
  int w = 0;
  int h = 0;
  std::vector<unsigned char> data;
  if (!LoadPng(imageFilename, data, w, h))
  {
    ReportError("Failed to load palette: " + imageFilename);
    return std::nullopt;
  }
  if (w == 0 || h == 0)
  {
    ReportError("Bad image size? " + imageFilename);
    return std::nullopt;
  }
  if (h > 1)
  {
    ReportError("Only 0th row will be sampled: " + imageFilename);
    // not a fatal error
  }
  auto end = data.begin();
  std::advance(end, w * 4);
  // Convert range of raw RGBA bytes to vec of Colours
  return RgbaToColourVec(data.begin(), end);
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

