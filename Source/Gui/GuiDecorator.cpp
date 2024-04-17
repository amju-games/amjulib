#include "GuiDecorator.h"

namespace Amju
{
bool GuiDecorator::Load(File* f) 
{ 
  return LoadOneChild(f); 
}

bool GuiDecorator::Save(File* f) 
{ 
  if (!SaveTypeAndName(f)) // TODO Not sure about this
  {
    return false;
  }
  return GetChild()->Save(f);
}

GuiElement* GuiDecorator::GetChild() 
{ 
  Assert(m_children.size() == 1);
  return GuiComposite::GetChild(0); 
}
}