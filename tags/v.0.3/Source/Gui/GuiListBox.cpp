#include "GuiListBox.h"
#include <EventPoller.h>
#include <AmjuGL.h>
#include <Screen.h>
#include <DrawRect.h>

namespace Amju
{
const char* GuiListBox::NAME = "gui-list-box";

GuiListBox::GuiListBox()
{
  m_isMultiSelect = false;
}

void GuiListBox::SetIsMultiSel(bool isMulti)
{
  m_isMultiSelect = isMulti;
}

bool GuiListBox::IsMultiSel() const
{
  return m_isMultiSelect;
}

void GuiListBox::Draw()
{
  if (!IsVisible())
  {
    return;
  }

  // Draw bg - TODO just the parts not covered by child items
  PushColour();
  AmjuGL::SetColour(0, 0, 1, 1);
  Rect r = GetRect(this);
  DrawSolidRect(r);
  PopColour();

  GuiWindow::Draw();
}

void GuiListBox::SetSelected(int child, bool selected)
{
  Assert(child < GetNumChildren());

////  ((GuiText*)GetChild(child))->SetInverse(selected); // TODO what if Items are not Text ?
  GetChild(child)->SetSelected(selected);

  if (!IsMultiSel())
  {
    // Not multi select, so at most one member in set.
    m_selset.clear();
  }

  if (selected)
  {
    m_selset.insert(child);
  }
  else
  {
    m_selset.erase(child);
  }
}

bool GuiListBox::IsSelected(int child) const
{
  return (m_selset.count(child) != 0);
}

int GuiListBox::GetSelectedItem() const
{
  // only for non-multi-select lists
  Assert(!IsMultiSel());

  int c = GetNumChildren(); 
  for (int i = 0; i < c; i++)
  {
    if (IsSelected(i))
    {
      return i;
    }
  }
  return -1;
}

void GuiListBox::AddItem(GuiText* text)
{
  text->SetDrawBg(true); // TODO TEMP TEST
  text->SetJust(GuiText::AMJU_JUST_LEFT); // ?
  text->SizeToText(); // Resize
  text->RecalcFirstLast();

  // Size to width of listbox
  Vec2f size = text->GetSize();
  size.x = GetSize().x;
  text->SetSize(size);

  Vec2f pos = GetLocalPos();
  // NB subtract => go down the screen
  // TODO This should be accumulated height
  pos.y -= 0.1f * (float)m_children.size(); // TODO
  text->SetLocalPos(pos);

  m_children.push_back(text);
}

bool GuiListBox::Load(File* f)
{
  // Get name, pos, size
  if (!GuiElement::Load(f))
  {
    return false;
  }

  // TODO Load strings

  return true;
}

bool GuiListBox::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  Vec2f cursorPos(mbe.x, mbe.y);
 
  if (IsVisible() &&
      mbe.button == AMJU_BUTTON_MOUSE_LEFT &&
      mbe.isDown)
  {
    if (!GetRect(this).IsPointIn(cursorPos) &&
        !(GetParent() && GetRect(GetParent()).IsPointIn(cursorPos)))
    {
      // Click outside area 
      //SetVisible(false);
      return false; // not handled 
    }

    // Clicked on an item
    // Check each item
    int selected = -1;
    for (unsigned int i = 0; i < m_children.size(); i++)
    {
      GuiElement* item = m_children[i];

      Rect r = GetRect(item);

      if (r.IsPointIn(cursorPos))
      {
        selected = i;
        break; 
      }
    }

    if (selected != -1)
    {
      // TODO Should react on mouse up when up item == down item.

      // Toggle selected flag for this child
      SetSelected(selected, !IsSelected(selected));

      //m_items[m_selected]->ExecuteCommand();
      // Problem with nested item, but should open up on mouse over anyway...
//      SetVisible(false); // TODO flag for this behaviour
      return true; // handled
    }
  }
  return false; // not handled
}

}
