// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <AmjuGL.h>
#include <LoadVec2.h>
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

  // 2 angles or one? (degrees)
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

  // Pivot point
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected pivot point for rotate decorator");
    return false;
  }
  ToVec2(s, &m_pivot);

  if (!GuiDecorator::Load(f))
  {
    return false;
  }

  SetName("rotate-" + m_children[0]->GetName());

  return true;
}

void GuiDecRotate::Draw()
{
  Vec2f childSize = m_children[0]->GetSize();

  // Convert relative pivot to absolute coord. 
  // E.g. (.5, .5) should be the centre of the element.
  Vec2f pos = m_children[0]->GetCombinedPos();
  Vec2f pivot = Vec2f(pos.x + m_pivot.x * childSize.x, pos.y - m_pivot.y * childSize.y);
  AmjuGL::PushMatrix();
  AmjuGL::Translate(pivot.x, pivot.y, 0);
  AmjuGL::RotateZ(m_interpAngle);
  AmjuGL::Translate(-pivot.x, -pivot.y, 0);
  GuiDecorator::Draw();
  AmjuGL::PopMatrix();
}

void GuiDecRotate::Animate(float animValue)
{
  m_interpAngle = m_angle[0] + (m_angle[1] - m_angle[0]) * animValue;

  AnimateChildren(animValue);
}

void GuiDecRotate::SetRotation(float angle, int zeroOrOne)
{
  m_angle[zeroOrOne] = angle;
  m_interpAngle = angle;
}

void GuiDecRotate::SetPivot(const Vec2f& pivot)
{
  m_pivot = pivot;
}

}

