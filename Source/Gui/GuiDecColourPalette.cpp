// * Amjulib *
// (c) Copyright 2000-2026 Juliet Colman

#include "GuiDecColourPalette.h"

namespace Amju
{
const char* GuiDecColourPalette::NAME;

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

  // Somehow, load the texture and convert its colours along the top row
  //  into a vector.
  //m_texure = TheResourceManager::Instance()->GetResource(m_paletteFilename);

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

