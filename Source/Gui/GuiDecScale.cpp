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

void GuiDecScale::Update()
{
  Vec2f pos = m_children[0]->GetLocalPos();

  // With this one, pivot of (0, 0) is the centre
  pos -= Vec2f(m_origChildSize.x * 0.5f * m_interpScale.x * (1.0f + m_pivot.x), -m_origChildSize.y * 0.5f * m_interpScale.y * (1.0f + m_pivot.y));

//  pos -= Vec2f(m_origChildSize.x *  m_interpScale.x * (1.0f - m_pivot.x), -m_origChildSize.y *  m_interpScale.y * (1.0f + m_pivot.y));
  SetLocalPos(pos);

  Vec2f size(m_origChildSize.x * m_interpScale.x, m_origChildSize.y * m_interpScale.y);
  m_children[0]->SetSize(size);
}

void GuiDecScale::Animate(float animValue)
{
  m_interpScale = m_scale[0] + (m_scale[1] - m_scale[0]) * animValue;

  AnimateChildren(animValue);
}
}
