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
    f->ReportError("Expected scale (1 or 2) for scale decorator");
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

  // Pivot point
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected pivot point for scale decorator");
    return false;
  }
  ToVec2(s, &m_pivot);

  if (!GuiDecorator::Load(f))
  {
    return false;
  }

  m_origChildSize = m_children[0]->GetSize();

  SetName("scale-" + m_children[0]->GetName());

  return true;
}

void GuiDecScale::SetScale(const Vec2f& scale, int zeroOrOne)
{
  m_scale[zeroOrOne] = scale;
  m_interpScale = scale;
}

void GuiDecScale::SetPivot(const Vec2f& pivot)
{
  m_pivot = pivot;
}

void GuiDecScale::Draw()
{
  AmjuGL::PushMatrix();

  // Convert relative pivot to absolute coord. 
  // E.g. (.5, .5) should be the centre of the element.
  Vec2f pos = m_children[0]->GetCombinedPos(); 
  Vec2f pivot = Vec2f(pos.x + m_pivot.x * m_origChildSize.x, pos.y - m_pivot.y * m_origChildSize.y);
  AmjuGL::Translate(pivot.x, pivot.y, 0);
  AmjuGL::Scale(m_interpScale.x, m_interpScale.y, 1);
  AmjuGL::Translate(-pivot.x, -pivot.y, 0);
  GuiDecorator::Draw();
  AmjuGL::PopMatrix();
}

void GuiDecScale::Animate(float animValue)
{
  m_interpScale = m_scale[0] + (m_scale[1] - m_scale[0]) * animValue;

  AnimateChildren(animValue);
}
}
