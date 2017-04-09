// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include <StringUtils.h>
#include "GuiDecColour.h"

namespace Amju
{
const char* GuiDecColour::NAME = "colour";

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
  m_colour[0] = FromHexString(strs[0]);
  if (strs.size() == 1)
  {
    m_colour[1] = m_colour[0];
  }
  else
  {
    m_colour[1] = FromHexString(strs[1]);
  }
  m_interpolatedColour = m_colour[0];

  if (!GuiDecorator::Load(f))
  {
    return false;
  }

  SetName("colour-" + m_children[0]->GetName());

  return true;
}

void GuiDecColour::Draw()
{
  PushColour();
  MultColour(m_interpolatedColour);

  GuiDecorator::Draw();

  PopColour();
}

void GuiDecColour::Animate(float animValue)
{
  m_interpolatedColour = Interpolate(m_colour[0], m_colour[1], animValue);

  AnimateChildren(animValue);
}

void GuiDecColour::SetColour(const Colour& col, int zeroOrOne)
{
  m_colour[zeroOrOne] = col;
}
}
