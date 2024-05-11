#ifndef GUI_MENU_H
#define GUI_MENU_H

#include <vector>
#include "GuiDecorator.h"
#include "GuiElement.h"
#include "GuiText.h"

namespace Amju
{
class GuiMenu;
typedef RCPtr<GuiMenu> PGuiMenu;


class GuiMenuItem : public GuiDecorator
{
public:
  virtual void OnRightClickMenuItem(GuiElement* elem);

  void SetRightClickCommand(CommandFunc command);
  void SetRightClickCommand(PGuiCommand command);

  void SetHighlighted(bool yesHighlighted);

  void SetEnabled(bool enable) { m_isEnabled = enable; }
  bool IsEnabled() const { return m_isEnabled; }

  void Draw() override;

protected:
  CommandFunc m_rightClickCommandFunc;
  PGuiCommand m_rightClickCommand;
  bool m_isEnabled;
};

class GuiTextMenuItem : public GuiMenuItem
{
public:
  // Size not specified - defaults to width of text when all displayed
  //  in one line

  GuiTextMenuItem() = default;

  GuiTextMenuItem(const std::string& text);
  GuiTextMenuItem(const std::string& text, CommandFunc commandFunc);
  GuiTextMenuItem(const std::string& text, PGuiCommand commandFunc);

  void Draw() override;

  std::string GetText();

protected:
  void Init(const std::string&);
  virtual GuiText* CreateTextChild();
};

class GuiTextEditMenuItem : public GuiTextMenuItem
{
public:
  GuiTextEditMenuItem(const std::string& text);
  void SetOnTextChangedCommand(CommandFunc onChanged);
  void OnTextChanged();

protected:
  GuiText* CreateTextChild() override;
  CommandFunc m_onTextChangedCommand;
};

typedef RCPtr<GuiMenuItem> PGuiMenuItem;

// For nested menus
class GuiNestMenuItem : public GuiTextMenuItem
{
public:
  GuiNestMenuItem(const std::string& text, GuiMenu* childMenu);

  virtual void Draw();
  GuiMenu* GetChild() { return m_childMenu; } // TODO don't need this, (and it hides non-v func): m_childMenu should be the child of this decorator.

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

private:
  PGuiMenu m_childMenu;
};


class GuiMenu : public GuiComposite
{
public:
  static const char* NAME;

  GuiMenu();
  virtual ~GuiMenu();

  void SetIsVertical(bool isVertical) { m_isVertical = isVertical; } 
  bool IsVertical() const { return m_isVertical; }

  void Draw() override;  

  void AddChild(GuiElement* pItem) override;
  //void Clear();
  // TODO Add Insert, Remove items: call GuiComposite funcs

  // Adjust selected item when cursor moves
  bool OnCursorEvent(const CursorEvent&) override;
  bool OnMouseButtonEvent(const MouseButtonEvent&) override;

  // Set callback for when mouse is clicked away from menu
  void SetOnClickedAwayFunc(CommandFunc);

protected:
  void HideChildren();

protected:
  // Selected menu item: index into vector of children, or -1 if no selection.
  int m_selected = -1; 

  // Highlight: index into vector of children.
  // Show cursor is over item, but don't select it unless left-clicked.
  int m_highlighted = -1; 

  Vec2f m_cursorPos; // last position passed into OnCursorEvent

//  PGuiMenu m_childMenu; // ?
  bool m_isVertical;

  // Callback when mouse click is outside menu bounds
  CommandFunc m_clickedAway;
};

// The kind of menu that moves around on screen, e.g. a right-click context menu.
class GuiFloatingMenu : public GuiMenu
{
public:
  // Hide the menu if we click off it
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
}

#endif

