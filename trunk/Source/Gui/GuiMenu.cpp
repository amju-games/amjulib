#include "GuiMenu.h"
#include <AmjuGL.h>
#include <Colour.h>
#include <DrawRect.h>
#include <EventPoller.h>

namespace Amju
{
static const int MENU_PRIORITY = -1; // higher priority

GuiMenuItem::GuiMenuItem(const std::string& text)
{
  Init(text);
}

void GuiMenuItem::Init(const std::string& text)
{
  SetText(text);
  SetDrawBg(true);
  SetJust(AMJU_JUST_LEFT);

  SizeToText();
}

GuiMenuItem::GuiMenuItem(const std::string& text, CommandFunc commandFunc)
{
  Init(text);
  SetCommand(commandFunc);
}

GuiMenuItem::GuiMenuItem(const std::string& text, PGuiCommand command)
{
  Init(text);
  SetCommand(command);
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
    GuiNestMenuItem* menuItem = (GuiNestMenuItem*)m_pGui;

    // Make sub menu visible
    menuItem->GetChild()->SetVisible(true);

    // Set Pos of nested menu: may be to right, or below this item
    // (Ignore to left for now, TODO)

    // Parent of this menu item may be the menu root, in which case it may be horiz or vertical.
    GuiMenu* topMenu = dynamic_cast<GuiMenu*>(menuItem->GetParent());
    if (topMenu && !topMenu->IsVertical())
    {
      // Draw below
      ((GuiNestMenuItem*)m_pGui)->GetChild()->SetLocalPos(/*m_pGui->GetPos() + */ Vec2f(0, -m_pGui->GetSize().y));
    }
    else
    {
      // Draw to right
      ((GuiNestMenuItem*)m_pGui)->GetChild()->SetLocalPos(/*m_pGui->GetPos() + */ Vec2f(m_pGui->GetSize().x, 0));
    }
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
  m_selected = -1;
  m_isVertical = true;
  m_hideOnSelection = true;

  TheEventPoller::Instance()->AddListener(this, MENU_PRIORITY);
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

  Vec2f pos; //(m_pos); // calc pos of each item

  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    bool isSel = (m_selected == (int)i);
    GuiElement* item = m_children[i];

    // Highlight selected item
////    ((GuiText*)item)->SetInverse(isSel); // TODO what if Items are not Text ?
    item->SetSelected(isSel);

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
    item->SetLocalPos(pos);
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

bool GuiMenu::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (IsVisible() &&
      mbe.button == AMJU_BUTTON_MOUSE_LEFT &&
      mbe.isDown)
  {
    if (!GetRect(this).IsPointIn(m_cursorPos) &&
        !(GetParent() && GetRect(GetParent()).IsPointIn(m_cursorPos)))
    {
      // Click outside menu area => hide menu
      SetVisible(false);
      return true; //  handled (for last time until made visible again)
    }

    // Clicked on an item
    if (m_selected != -1)
    {
      // TODO Should react on mouse up when up item == down item.
      m_children[m_selected]->ExecuteCommand();
      // Problem with nested item, but should open up on mouse over anyway...
      if (m_hideOnSelection)
      {
        SetVisible(false); 
      }
      return true; // handled
    }
  }
  return false; // not handled
}

bool GuiMenu::OnCursorEvent(const CursorEvent& ce)
{
  m_cursorPos.x = ce.x;
  m_cursorPos.y = ce.y;

  if (!IsVisible())
  {
    return false; // not handled
  }

  m_selected = -1;
  if (m_children.empty())
  {
    return false;
  }

  // Check each item
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    GuiElement* item = m_children[i];

    Rect r = GetRect(item);

    if (r.IsPointIn(Vec2f(ce.x, ce.y)))
    {
      m_selected = i;
      return true; // handled
    }
  }
  return false;
}

void GuiMenu::AddChild(GuiElement* pItem) // overrides GuiComposite
{
  m_children.push_back(pItem);
  pItem->SetParent(this);

  // Adjust size of menu
  const Vec2f& size = pItem->GetSize();
  Vec2f thisSize = GetSize();

  if (m_isVertical)
  {
    if (size.x > thisSize.x)
    {
      thisSize.x = size.x;
    }
    thisSize.y += size.y;
  }
  else
  {
    if (size.y > thisSize.y)
    {
      thisSize.y = size.y;
    }
    thisSize.x += size.x;
  }

  SetSize(thisSize);
}

//void GuiMenu::Clear()
//{
//   m_items.clear();
//}
}
