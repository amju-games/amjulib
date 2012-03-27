#include "GuiListBox.h"

namespace Amju
{
const char* GuiListBox::NAME = "gui-list-box";

void GuiListBox::AddItem(GuiText* text)
{
  m_children.push_back(text);
}
}

