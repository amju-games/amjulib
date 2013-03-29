#include <AmjuFirst.h>
#include "GuiMenu.h"
#include <AmjuGL.h>
#include <Colour.h>
#include <DrawRect.h>
#include <EventPoller.h>
#include <AmjuFinal.h>

namespace Amju
{
const char* GuiMenu::NAME = "gui-menu";

GuiMenuItem::GuiMenuItem(const std::string& text)
{
  Init(text);
}

void GuiMenuItem::Init(const std::string& text)
{
  SetText(text);
  SetDrawBg(true);
  //std::swap(m_bgCol, m_fgCol); // becase draw BG inverts???

  SetJust(AMJU_JUST_LEFT);

  SizeToText();
  RecalcFirstLast();
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
    GuiElement* child = menuItem->GetChild();
    child->SetVisible(true);

    // Set Pos of nested menu: may be to right, or below this item
    // (Ignore to left for now, TODO)

    // Parent of this menu item may be the menu root, in which case it may be horiz or vertical.
    GuiMenu* topMenu = dynamic_cast<GuiMenu*>(menuItem->GetParent());
    if (topMenu && !topMenu->IsVertical())
    {
      // Draw below
      child->SetLocalPos(/*m_pGui->GetPos() + */ Vec2f(0, -m_pGui->GetSize().y));
    }
    else
    {
      // Draw to left/right
      float posx = m_pGui->GetCombinedPos().x;
      float x = m_pGui->GetSize().x;
      float childw = child->GetSize().x;
      if ((posx + x + childw) > 1.0f)
      {
        x = -childw;
      }
      float y = 0;
      float posy = m_pGui->GetCombinedPos().y;
      float childh = child->GetSize().y;
      if ((posy - childh) < -1.0f)
      {
        y = -1.0f - (posy - childh);
      }
      
      // Set rel pos
      child->SetLocalPos(Vec2f(x, y));
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

bool GuiNestMenuItem::OnCursorEvent(const CursorEvent& ce)
{
  if (m_childMenu) 
  {
    return m_childMenu->OnCursorEvent(ce);
  }
  return false;
}

bool GuiNestMenuItem::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (m_childMenu) 
  {
    return m_childMenu->OnMouseButtonEvent(mbe);
  }
  return false;
}

GuiMenu::GuiMenu()
{
  m_selected = -1;
  m_isVertical = true;
  m_clickedAway = 0;
}

GuiMenu::~GuiMenu()
{
  // Done in ~EventListener
  //TheEventPoller::Instance()->RemoveListener(this); 
}

void GuiMenu::SetOnClickedAwayFunc(CommandFunc func)
{
  m_clickedAway = func;
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
  GuiComposite::OnMouseButtonEvent(mbe);

  if (IsVisible() &&
      mbe.button == AMJU_BUTTON_MOUSE_LEFT &&
      mbe.isDown)
  {
    Rect r = GetRect(this);
    if (!r.IsPointIn(m_cursorPos) 
      //&& !(GetParent() && GetRect(GetParent()).IsPointIn(m_cursorPos)) // TODO
    )
    {
      // TODO Not if this is a fixed menu bar

      // Click outside menu area => hide menu
      SetVisible(false);
      // Callback for clicking off menu  
      if (m_clickedAway)
      {
        m_clickedAway();
      }

      // TODO Callback for when menu is made invis

      return true; //  handled (for last time until made visible again)
    }

    // Clicked on an item
    if (m_selected != -1)
    {
      // TODO Should react on mouse up when up item == down item.
      GuiElement* child = m_children[m_selected];
      child->ExecuteCommand();

      // If not nested, 
      bool hideOnSelection = (dynamic_cast<GuiNestMenuItem*>(child) == 0);
      if (hideOnSelection)
      {
        SetVisible(false); 
      }
      return true; // handled
    }
  }

  // If mouse button is up, and over menu, treat as handled. Right ?
  if (IsVisible() &&
      mbe.button == AMJU_BUTTON_MOUSE_LEFT &&
      !mbe.isDown)
  {
    Rect r = GetRect(this);
    if (r.IsPointIn(m_cursorPos))
    {
      return true;
    }
  }

  return false; // not handled
}

bool GuiMenu::OnCursorEvent(const CursorEvent& ce)
{
  GuiComposite::OnCursorEvent(ce);

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
