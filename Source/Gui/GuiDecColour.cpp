// * Amjulib *
// (c) Copyright 2000-2017 Juliet Colman

#include <StringUtils.h>
#include "GuiDecColour.h"

namespace Amju
{
const char* GuiDecColour::NAME = "colour";

bool GuiDecColour::Save(File* f)
{
  std::string s = ToHexString(m_colour);
  if (m_colour != m_secondColour)
  {
    s += ", " + ToHexString(m_secondColour);
  }
  if (!f->Write(GetTypeName()))
  {
    return false;
  }
  if (!f->Write(s))
  {
    return false;
  }
  return GetChild()->Save(f);
}

bool GuiDecColour::Load(File* f)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected colour info");
    return false;
  }

  // Allow this string to be "clean" - TODO think of better name.
  // If "clean", we set the colour on our child, we don't multiply by the 
  // ancestor colour.
  
  // 2 colours or one?
  Strings strs = Split(s, ',');
  Assert(!strs.empty());

  // Check for "reset" string, to set instead of multiplying colour.
  auto it = std::find(strs.begin(), strs.end(), "reset");
  if (it != strs.end())
  {
    m_mode = DEC_COLOUR_RESET;
    strs.erase(it); // remove to simplify parsing below
  }

  // Check for "add" string, to set additive mode.
  it = std::find(strs.begin(), strs.end(), "add");
  if (it != strs.end())
  {
    m_mode = DEC_COLOUR_ADD;
    strs.erase(it); // remove to simplify parsing below
  }

  auto optionalColour = FromHexString(strs[0]);
  if (optionalColour)
  {
    m_colour = *optionalColour;
  }
  else
  {
    f->ReportError("Bad colour: \"" + strs[0] + "\"");
    return false;
  }

  if (strs.size() == 1)
  {
    m_secondColour = m_colour;
  }
  else
  {
    auto optionalColour = FromHexString(strs[1]);
    if (optionalColour)
    {
      m_secondColour = *optionalColour;
    }
    else
    {
      f->ReportError("Bad colour: \"" + strs[1] + "\"");
      return false;
    }
  }
  m_interpolatedColour = m_colour;

  if (!GuiDecorator::Load(f))
  {
    return false;
  }

  SetName("colour-" + GetChild()->GetName());

  return true;
}

void GuiDecColour::Draw()
{
  // Don't draw if completely transparent. This lets us use this decorator
  //  as a visibility switch without overdraw cost.
  if (m_interpolatedColour.m_a < .0001f)
  {
    return;
  }

  // TODO Don't do this, use vertex colours: BATCH!
  PushColour();

  // Reset colour or multiply ancestor colour
  switch (m_mode)
  {
  case Amju::GuiDecColour::DEC_COLOUR_MULT:
    MultColour(m_interpolatedColour);
    break;
  case Amju::GuiDecColour::DEC_COLOUR_RESET:
    Amju::SetColour(m_interpolatedColour);
    break;
  case Amju::GuiDecColour::DEC_COLOUR_ADD:
    AddColour(m_interpolatedColour);
    break;
  default:
    break;
  }

  GuiDecorator::Draw();

  PopColour();
}

void GuiDecColour::Animate(float animValue)
{
  m_interpolatedColour = Interpolate(m_colour, m_secondColour, animValue);

  AnimateChildren(animValue);
}

void GuiDecColour::SetSecondColour(const Colour& colour)
{
  m_secondColour = colour;
  m_interpolatedColour = m_secondColour;
}

Colour GuiDecColour::GetColour() const
{
  return m_interpolatedColour;
}

}
