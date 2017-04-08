// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <AmjuGL.h>
#include <LoadVec2.h>
#include <StringUtils.h>
#include "GuiDecScale.h"

namespace Amju
{
const char* GuiDecScale::NAME = "scale";

bool GuiDecScale::Load(File* f)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected position for scale decorator");
    return false;
  }

  Strings strs = Split(s, ';');
  Assert(!strs.empty());

  ToVec2(strs[0], &m_scale[0]);
  if (strs.size() == 1)
  {
    m_scale[1] = m_scale[0];
  }
  else
  {
    ToVec2(strs[1], &m_scale[1]);
  }
  m_interpScale = m_scale[0];
  return GuiDecorator::Load(f);
}

void GuiDecScale::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::Scale(m_interpScale.x, m_interpScale.y, 1.f);
  GuiDecorator::Draw();
  AmjuGL::PopMatrix();
}

void GuiDecScale::Animate(float animValue)
{
  m_interpScale = m_scale[0] + (m_scale[1] - m_scale[0]) * animValue;

  AnimateChildren(animValue);
}
}
