#include "GuiMenu.h"
#include <AmjuGL.h>
#include <Colour.h>
#include <DrawRect.h>
#include <EventPoller.h>

namespace Amju
{
GuiMenuItem::GuiMenuItem(const std::string& text, CommandFunc commandFunc)
{
  SetText(text);
  SetDrawBg(true);
  SetJust(AMJU_JUST_LEFT);

  m_size.x = m_textWidth;
  m_size.y = 0.1f; // TODO TEMP TEST

  SetCommand(commandFunc);
}

void GuiMenuItem::Draw()
{
  if (!IsVisible())
  {
    return;
  }

  // If selected, reverse colours
  GuiText::Draw();
}

struct NestedMenuCommand : public GuiCommand
{
  virtual bool Do()
  {
    Assert(dynamic_cast<GuiNestMenuItem*>(m_pGui));
    ((GuiNestMenuItem*)m_pGui)->GetChild()->SetVisible(true);
    // Pos of nested menu may be to right, or below this item
    // (Ignore to left for now, TODO)
    ((GuiNestMenuItem*)m_pGui)->GetChild()->SetPos(m_pGui->GetPos() + Vec2f(m_pGui->GetSize().x, 0));
    return false;
  }
};

GuiNestMenuItem::GuiNestMenuItem(const std::string& text, GuiMenu* childMenu) :
  GuiMenuItem(text)
{
  m_childMenu = childMenu;
  m_childMenu->SetVisible(false);
  m_childMenu->SetParent(this);

  NestedMenuCommand* command = new NestedMenuCommand;
  command->SetGuiElement(this);
  SetCommand(command);
}

void GuiNestMenuItem::Draw()
{
  GuiMenuItem::Draw();
  if (IsVisible() && m_childMenu)
  {
    m_childMenu->Draw();
  }
}

GuiMenu::GuiMenu()
{
  m_parent = 0;
  m_selected = -1;
  m_isVertical = true;
  TheEventPoller::Instance()->AddListener(this);
}

GuiMenu::~GuiMenu()
{
  // Done in ~EventListener
  //TheEventPoller::Instance()->RemoveListener(this); 
}

void GuiMenu::Draw()
{
  if (!IsVisible())
  {
    return;
  }

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

  Vec2f pos(m_pos); // calc pos of each item

  for (unsigned int i = 0; i < m_items.size(); i++)
  {
    bool isSel = (m_selected == (int)i);
    GuiElement* item = m_items[i];

    // Highlight selected item
    ((GuiText*)item)->SetInverse(isSel); // TODO what if Items are not Text ?

    Vec2f size = item->GetSize();
    if (m_isVertical)
    {
      // Set width of all items to width of menu
      size.x = GetSize().x;
    }
    else
    {
      // Set height of all items to height of menu
      size.y = GetSize().y;
    }
    item->SetSize(size); 

    // Set position of item
    item->SetPos(pos);
    if (m_isVertical)
    {
      pos.y -= size.y; // -ve = down the screen
    }
    else
    {
      pos.x += size.x;
    }

    item->Draw();
  }
}

void GuiMenu::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (IsVisible() &&
      mbe.button == AMJU_BUTTON_MOUSE_LEFT &&
      mbe.isDown)
  {
    if (!GetRect(this).IsPointIn(m_cursorPos) &&
        !(m_parent && GetRect(m_parent).IsPointIn(m_cursorPos)))
    {
      // Click outside menu area => hide menu
      SetVisible(false);
      return;
    }

    // Clicked on an item
    if (m_selected != -1)
    {
      // TODO Should react on mouse up when up item == down item.
      m_items[m_selected]->ExecuteCommand();
    }
  }
}

void GuiMenu::OnCursorEvent(const CursorEvent& ce)
{
  m_cursorPos.x = ce.x;
  m_cursorPos.y = ce.y;

  m_selected = -1;
  if (m_items.empty())
  {
    return;
  }

  // Check each item
  for (unsigned int i = 0; i < m_items.size(); i++)
  {
    GuiElement* item = m_items[i];

    Rect r = GetRect(item);

    if (r.IsPointIn(Vec2f(ce.x, ce.y)))
    {
      m_selected = i;
      return;
    }
  }
}

void GuiMenu::AddItem(GuiMenuItem* pItem)
{
  m_items.push_back(pItem);

  // Adjust size of menu
  const Vec2f& size = pItem->GetSize();

  if (m_isVertical)
  {
    if (size.x > m_size.x)
    {
      m_size.x = size.x;
    }
    m_size.y += size.y;
  }
  else
  {
    if (size.y > m_size.y)
    {
      m_size.y = size.y;
    }
    m_size.x += size.x;
  }
}

void GuiMenu::Clear()
{
   m_items.clear();
}
}
