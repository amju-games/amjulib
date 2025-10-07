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

  // 2 colours or one?
  Strings strs = Split(s, ',');
  Assert(!strs.empty());
  auto optionalColour = FromHexString(strs[0]);
  if (optionalColour)
  {
    m_colour = *optionalColour;
  }
  else
  {
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
      return false;
    }
  }
  m_interpolatedColour = m_colour;

  if (!GuiDecorator::Load(f))
  {
    return false;
  }

  SetName("colour-" + m_children[0]->GetName());

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
  MultColour(m_interpolatedColour);

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
