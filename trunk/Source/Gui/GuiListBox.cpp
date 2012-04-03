#include "GuiListBox.h"
#include <EventPoller.h>
#include <AmjuGL.h>
#include <Screen.h>
#include <DrawRect.h>

namespace Amju
{
const char* GuiListBox::NAME = "gui-list-box";

static const int LB_PRIORITY = -1; // higher priority

GuiListBox::GuiListBox()
{
  m_isMultiSelect = true;
  TheEventPoller::Instance()->AddListener(this, LB_PRIORITY);
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

  int vp[4];
  AmjuGL::GetViewport(&vp[0], &vp[1], &vp[2], &vp[3]);

  SetViewportN(m_pos.x, m_pos.y -  m_size.y, m_size.x, m_size.y);

  // LB should fill whole viewport
  AmjuGL::PushMatrix();
  AmjuGL::Translate(m_pos.x * -2.0f / m_size.x - 1.0f, 1.0f - m_pos.y * 2.0f / m_size.y, 0);
  AmjuGL::Scale(2.0f / m_size.x, 2.0f / m_size.y, 1.0f);

  GuiComposite::Draw();
  AmjuGL::PopMatrix();
  AmjuGL::Viewport(vp[0], vp[1], vp[2], vp[3]);
}

void GuiListBox::SetSelected(int child, bool selected)
{
  Assert(child < GetNumChildren());

////  ((GuiText*)GetChild(child))->SetInverse(selected); // TODO what if Items are not Text ?
  GetChild(child)->SetSelected(selected);

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

void GuiListBox::AddItem(GuiText* text)
{
  text->SetDrawBg(true); // TODO TEMP TEST
  text->SetJust(GuiText::AMJU_JUST_LEFT); // ?
  text->SizeToText(); // Resize

  // Size to width of listbox
  Vec2f size = text->GetSize();
  size.x = GetSize().x;
  text->SetSize(size);

  Vec2f pos = GetPos();
  // NB subtract => go down the screen
  // TODO This should be accumulated height
  pos.y -= 0.1f * (float)m_children.size(); // TODO
  text->SetPos(pos);

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
        !(m_parent && GetRect(m_parent).IsPointIn(cursorPos)))
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

