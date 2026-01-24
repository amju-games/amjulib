// * Amjulib *
// (c) Copyright 2000-2026 Juliet Colman

#include "ColourMode.h"

namespace Amju
{
std::string ColourMode::GetStringFromMode() const
{
  switch (m_mode)
  {
  case DEC_COLOUR_MULT:
    return "";
  case DEC_COLOUR_RESET:
    return ", reset";
  case DEC_COLOUR_ADD:
    return ", add";
  }
  return "";
}

void ColourMode::SetModeFromStrings(Strings& strs)
{
  // Check for "reset" string
  auto it = std::find(strs.begin(), strs.end(), "reset");
  if (it != strs.end())
  {
    m_mode = DEC_COLOUR_RESET;
    strs.erase(it); // remove to simplify parsing in client code
  }

  // Check for "add" string
  it = std::find(strs.begin(), strs.end(), "add");
  if (it != strs.end())
  {
    m_mode = DEC_COLOUR_ADD;
    strs.erase(it); // remove to simplify parsing 
  }
}

void ColourMode::DrawColour(const Colour& c) const
{
 // Reset colour or multiply ancestor colour
  switch (m_mode)
  {
  case DEC_COLOUR_MULT:
    MultColour(c);
    break;
  case DEC_COLOUR_RESET:
    Amju::SetColour(c);
    break;
  case DEC_COLOUR_ADD:
    AddColour(c);
    break;
  }
}
}

