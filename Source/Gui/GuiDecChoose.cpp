// * Amjulib *
// (c) Copyright 2000-2026 Juliet Colman

#include <algorithm>
#include "GuiDecChoose.h"

namespace Amju
{
const char* GuiDecChoose::NAME = "choose";

bool GuiDecChoose::Load(File* f) 
{
  if (!GuiComposite::Load(f))
  {
    return false;
  }
  if (m_children.empty())
  {
    f->ReportError("GuiDecChoose must have at least one child to select from.\n");
    return false;
  }
  m_allChildren = m_children;
  // In case Animate() is not called early enough, set a default child
  m_children = { m_allChildren[m_chosenChild] };
  return true;
}

bool GuiDecChoose::Save(File* f) 
{
  // Copy all children so we save as normal
  m_children = m_allChildren;
  if (!GuiComposite::Save(f))
  {
    return false;
  }
  // Restore children to the chosen one only
  m_children = { m_allChildren[m_chosenChild] };
  return true;
}
 
void GuiDecChoose::Animate(float animValue) 
{
  m_chosenChild = std::clamp(
    static_cast<int>(animValue * m_allChildren.size()), 0, 
    static_cast<int>(m_allChildren.size() - 1));
  m_children = { m_allChildren[m_chosenChild] };
}
}

