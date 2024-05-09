#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Colour.h>
#include <DrawRect.h>
#include <EventPoller.h>
#include "GuiMenu.h"
#include "GuiTextEdit.h"
#include <AmjuFinal.h>

namespace Amju
{
const char* GuiMenu::NAME = "gui-menu";

void GuiMenuItem::Draw()
{
  GetChild()->SetSelected(IsSelected());

  // GuiMenu::Draw has correctly set the size of this item, but as we are a wrapper 
  //  around the "real" text element (or could be an image, etc), set the size of the
  //  child to the same size as this.
  GetChild()->SetSize(GetSize());

  GuiDecorator::Draw();
}

GuiTextMenuItem::GuiTextMenuItem(const std::string& text)
{
  Init(text);
}

void GuiTextMenuItem::Init(const std::string& text)
{
  SetName(text);

  GuiText* guiText = CreateTextChild();
  guiText->SetText(text);
  guiText->SetDrawBg(true);

  guiText->SetJust(GuiText::AMJU_JUST_LEFT);

  guiText->SizeToText();
  SetSize(guiText->GetSize());
  guiText->RecalcFirstLast();
  AddChild(guiText);
}

GuiTextMenuItem::GuiTextMenuItem(const std::string& text, CommandFunc commandFunc)
{
  Init(text);
  SetCommand(commandFunc);
}

GuiTextMenuItem::GuiTextMenuItem(const std::string& text, PGuiCommand command)
{
  Init(text);
  SetCommand(command);
}
 
std::string GuiTextMenuItem::GetText()
{
  GuiElement* child = GetChild();
  Assert(child);
  GuiText* textElem = dynamic_cast<GuiText*>(child);
  Assert(textElem);
  return textElem->GetText();
}

GuiText* GuiTextMenuItem::CreateTextChild()
{
  return new GuiText;
}

void GuiTextMenuItem::Draw()
{
  if (!IsVisible())
  {
    return;
  }
  
  // TODO Set colours for highlighted/selected text.

  GuiMenuItem::Draw();
}

static void OnEnterKeyOnTextEditMenuItem(GuiElement* elem)
{
  GuiTextEdit* textEdit = dynamic_cast<GuiTextEdit*>(elem);
  Assert(textEdit);
  textEdit->SetHasFocus(false);
  TheEventPoller::Instance()->RemoveListener(textEdit);
  auto parent = textEdit->GetParent();
  Assert(parent);
  GuiTextEditMenuItem* textEditMenuItem = dynamic_cast<GuiTextEditMenuItem*>(parent);
  Assert(textEditMenuItem);
  textEditMenuItem->OnTextChanged();
}

static void OnClickTextEditMenuItem(GuiElement* elem)
{
  GuiMenuItem* menuItem = dynamic_cast<GuiMenuItem*>(elem);
  Assert(menuItem);
  GuiElement* child = menuItem->GetChild();
  Assert(child);
  GuiTextEdit* textEdit = dynamic_cast<GuiTextEdit*>(child);
  Assert(textEdit);
  textEdit->SetHasFocus(true);
  TheEventPoller::Instance()->AddListener(textEdit);
  textEdit->SetCommand(OnEnterKeyOnTextEditMenuItem);
}

GuiTextEditMenuItem::GuiTextEditMenuItem(const std::string& text) 
{
  Init(text); // watch out for v function calls in ctor
  SetCommand(OnClickTextEditMenuItem);
}

GuiText* GuiTextEditMenuItem::CreateTextChild()
{
  return new GuiTextEdit;
}

void GuiTextEditMenuItem::SetOnTextChangedCommand(CommandFunc onChanged)
{
  m_onTextChangedCommand = onChanged;
}

void GuiTextEditMenuItem::OnTextChanged()
{
  Assert(m_onTextChangedCommand);
  m_onTextChangedCommand(this);
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
  GuiTextMenuItem(text)
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
    bool b = m_childMenu->OnMouseButtonEvent(mbe);
    return b;
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

  Vec2f pos;

  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    bool isSelected = (m_selected == static_cast<int>(i));

    GuiElement* item = m_children[i];

    // Invert selected item
    item->SetSelected(isSelected);

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

bool GuiFloatingMenu::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  bool b = GuiMenu::OnMouseButtonEvent(mbe);

  if (!b &&
      IsVisible() &&
      mbe.button == AMJU_BUTTON_MOUSE_LEFT &&
      mbe.isDown)
  {
    Rect r = GetRect(this);
    if (!r.IsPointIn(m_cursorPos))
    {
      // Click outside menu area => hide menu
      SetVisible(false);
      // Callback for clicking off menu  
      if (m_clickedAway)
      {
        m_clickedAway(this);
      }

      // TODO Callback for when menu is made invis
      return true; //  handled (for last time until made visible again)
    } 
  }
  return b;
}

bool GuiMenu::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  bool b = GuiComposite::OnMouseButtonEvent(mbe);

  if (IsVisible() &&
      mbe.button == AMJU_BUTTON_MOUSE_LEFT &&
      mbe.isDown)
  {
    // Not sure if this is what we want every time - TODO check this, maybe have a flag.
    // (Fix for lingering sub menus in AmjuWW edit mode)
    if (m_selected == -1)
    {
////      SetVisible(false);  // Hide if clicked away
    }
    else
    {
      // Left Clicked on an item
      // TODO Should react on mouse up when up item == down item.
      GuiElement* child = m_children[m_selected];
      child->ExecuteCommand();

      // If not nested, 
      bool hideOnSelection = (dynamic_cast<GuiNestMenuItem*>(child) == 0);
      if (hideOnSelection)
      {
////        SetVisible(false); 
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

  if (IsVisible() &&
    mbe.button == AMJU_BUTTON_MOUSE_RIGHT &&
    !mbe.isDown)
  {
    if (m_selected > -1)
    {
      GuiElement* child = m_children[m_selected];
      GuiMenuItem* menuItem = dynamic_cast<GuiMenuItem*>(child);
      if (menuItem)
      {
        menuItem->OnRightClickMenuItem(menuItem);
      }
      return true;
    }
  }
  if (IsVisible() &&
    mbe.button == AMJU_BUTTON_MOUSE_RIGHT &&
    mbe.isDown &&
    m_selected > -1)
  {
    return true;
  }

  // Not handled but may be handled by a child
  return b; 
}

bool GuiMenu::OnCursorEvent(const CursorEvent& ce)
{
  if (!IsVisible())
  {
    return false; // not handled
  }

  bool b = GuiComposite::OnCursorEvent(ce);

  m_cursorPos.x = ce.x;
  m_cursorPos.y = ce.y;

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

  //if (b)
  //{
  //  HideChildren(); // Not sure, but if we are on this menu, we should hide submenus?
  //}

  return b;
}

void GuiMenu::HideChildren()
{
  int n = GetNumChildren();
  for (int i = 0; i < n; i++)
  {
    GetChild(i)->SetVisible(false);
  }
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

void GuiMenuItem::OnRightClickMenuItem(GuiElement* elem)
{
  if (m_rightClickCommand)
  {
    m_rightClickCommand->SetGuiElement(elem);
    m_rightClickCommand->Do();
  }
  else if (m_rightClickCommandFunc)
  {
    m_rightClickCommandFunc(elem);
  }
}

void GuiMenuItem::SetRightClickCommand(CommandFunc command)
{
  m_rightClickCommandFunc = command;
  m_rightClickCommand = nullptr;
}

void GuiMenuItem::SetRightClickCommand(PGuiCommand command)
{
  m_rightClickCommand = command;
  m_rightClickCommandFunc = nullptr;
}
}
