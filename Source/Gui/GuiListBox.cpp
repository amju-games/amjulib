#include "GuiListBox.h"
#include <EventPoller.h>
#include <AmjuGL.h>
#include <Screen.h>
#include <DrawRect.h>
#include <AmjuAssert.h>
#include "GuiScroll.h"

namespace Amju
{
const char* GuiListBox::NAME = "gui-list-box";

GuiElement* CreateListBox() 
{
  // To make list boxes scroll, create a Scroll decorator which has a List Box child.
  // Stick the whole thing in a Window.

  GuiListBox* w = new GuiListBox;
  GuiScroll* s = new GuiScroll;
  GuiList* lb = new GuiList;
  w->AddChild(s);
  s->AddChild(lb);
  return w; 
}

GuiListBox::GuiListBox()
{
}

void GuiListBox::Clear()
{
  GetList()->Clear();
}

GuiList* GuiListBox::GetList()
{
  GuiElement* child = GetChild(0);
  Assert(child);
  GuiScroll* scr = dynamic_cast<GuiScroll*>(child);
  Assert(scr);
  GuiElement* gr = scr->GetChild(0);
  Assert(gr);
  GuiList* list = dynamic_cast<GuiList*>(gr);
  Assert(list);
  return list;
}

GuiScroll* GuiListBox::GetScroll()
{
  GuiElement* child = GetChild(0);
  Assert(child);
  GuiScroll* scr = dynamic_cast<GuiScroll*>(child);
  Assert(scr);
  return scr;
}

bool GuiListBox::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }
//  GetScroll()->SetLocalPos(GetCombinedPos());
  GetScroll()->SetSize(GetSize()); // TODO do we need this ?
//  GetList()->SetLocalPos(GetCombinedPos());
  GetList()->SetSize(GetSize());

  return true;
}

GuiList::GuiList()
{
  m_isMultiSelect = false;
  m_doubleClickFunc = 0;
  m_singleClickFunc = 0;
}

void GuiList::Clear()
{
  GuiComposite::Clear();
  m_selset.clear();
}

void GuiList::SetDoubleClickFunc(ClickFunc cf)
{
  m_doubleClickFunc = cf;
}

void GuiList::SetSingleClickFunc(ClickFunc cf)
{
  m_singleClickFunc = cf;
}

void GuiList::SetIsMultiSel(bool isMulti)
{
  m_isMultiSelect = isMulti;
}

bool GuiList::IsMultiSel() const
{
  return m_isMultiSelect;
}

void GuiList::Draw()
{
  if (!IsVisible())
  {
    return;
  }

/*
  Rect r = GetRect(this);

  // Draw bg - TODO just the parts not covered by child items
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  PushColour();
  AmjuGL::SetColour(0, 0, 1, 1);
  DrawSolidRect(r);
  PopColour();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
*/

  GuiComposite::Draw();

/*
  // Border
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  PushColour();
  AmjuGL::SetColour(1, 1, 1, 1);
  DrawRect(r);
  PopColour();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
*/
}

void GuiList::SetSelected(int child, bool selected)
{
  Assert(child < GetNumChildren());

#ifdef _DEBUG
  GuiText* text = dynamic_cast<GuiText*>(GetChild(child));
  if (text)
  {
    std::cout << "List box " << m_name << ": " << text->GetText() << (selected ? " selected" : " UNselected") << "\n";
  }
#endif

  if (selected && !IsMultiSel())
  {
    // Not multi select, so at most one member in set.
    for (SelSet::iterator it = m_selset.begin(); it != m_selset.end(); ++it)
    {
      int i = *it;
      GetChild(i)->SetSelected(false);
    }
    m_selset.clear();
  }

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

bool GuiList::IsSelected(int child) const
{
  return (m_selset.count(child) != 0);
}

int GuiList::GetSelectedItem() const
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

void GuiList::AddItem(GuiText* text)
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

  AddChild(text);
}

bool GuiList::Load(File* f)
{
  // Get name, pos, size
  if (!GuiElement::Load(f))
  {
    return false;
  }

  // TODO Load strings

  return true;
}

bool GuiList::OnDoubleClickEvent(const DoubleClickEvent& dce)
{
  if (dce.button != AMJU_BUTTON_MOUSE_LEFT)
  {
    return false;
  }

  if (!IsVisible())
  {
    return false;
  }

  if (m_doubleClickFunc)
  {
    Vec2f cursorPos(dce.x, dce.y);
    for (unsigned int i = 0; i < m_children.size(); i++)
    {
      GuiElement* item = m_children[i];
      Rect r = GetRect(item);
      if (r.IsPointIn(cursorPos))
      {
        m_doubleClickFunc(this, i);
        return true; 
      }
    }
  }

  return false;
}

bool GuiList::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (!IsVisible())
  {
    return false;
  }

  Vec2f cursorPos(mbe.x, mbe.y);
 
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT &&
      mbe.isDown)
  {
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
      // TODO Should react on mouse up when up item == down item ??

      if (IsMultiSel())
      {
        // Toggle selected flag for this child
        SetSelected(selected, !IsSelected(selected));
      }
      else
      {
        SetSelected(selected, true);
        if (m_singleClickFunc)
        {
          m_singleClickFunc(this, selected);
        }
      }

      // TODO Why is this a bad idea ?
      //m_items[m_selected]->ExecuteCommand(); 

      // Problem with nested item, but should open up on mouse over anyway...
//      SetVisible(false); // TODO flag for this behaviour
      return true; // handled
    }
  }
  return false; // not handled
}

}

