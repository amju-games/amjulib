// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <AmjuGL.h>
#include <StringUtils.h>
#include "GuiDecRotate.h"

namespace Amju
{
const char* GuiDecRotate::NAME = "rotate";

bool GuiDecRotate::Load(File* f)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected angle(s) for rotate decorator");
    return false;
  }

  // 2 colours or one?
  Strings strs = Split(s, ',');
  Assert(!strs.empty());
  m_angle[0] = ToFloat(strs[0]);
  if (strs.size() == 1)
  {
    m_angle[1] = m_angle[0];
  }
  else
  {
    m_angle[1] = ToFloat(strs[1]);
  }
  m_interpAngle = m_angle[0];

  return GuiDecorator::Load(f);
}

void GuiDecRotate::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::RotateZ(m_interpAngle);
  GuiDecorator::Draw();
  AmjuGL::PopMatrix();
}

void GuiDecRotate::Animate(float animValue)
{
  m_interpAngle = m_angle[0] + (m_angle[1] - m_angle[0]) * animValue;

  std::cout << "Interp angle: " << m_interpAngle << "\n";

  AnimateChildren(animValue);
}
}

