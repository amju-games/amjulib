// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <LoadVec2.h>
#include <StringUtils.h>
#include "GuiDecTranslate.h"

namespace Amju
{
const char* GuiDecTranslate::NAME = "translate";

bool GuiDecTranslate::Load(File* f)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected position for translate decorator");
    return false;
  }

  Strings strs = Split(s, ';');
  Assert(!strs.empty());

  ToVec2(strs[0], &m_positions[0]);
  if (strs.size() == 1)
  {
    m_positions[1] = m_positions[0];
  }
  else
  {
    ToVec2(strs[1], &m_positions[1]);
  }
  SetLocalPos(m_positions[0]);

  if (!GuiDecorator::Load(f))
  {
    return false;
  }

  SetName("translate-" + m_children[0]->GetName());

  return true;
}

void GuiDecTranslate::SetTranslation(const Vec2f& tr, int zeroOrOne)
{
  m_positions[zeroOrOne] = tr;
  SetLocalPos(tr);
}

void GuiDecTranslate::Animate(float animValue)
{
  Vec2f interp = m_positions[0] + (m_positions[1] - m_positions[0]) * animValue;
  SetLocalPos(interp);

  AnimateChildren(animValue);
}
}

