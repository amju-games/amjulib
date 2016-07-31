#ifndef GUI_MENU_H
#define GUI_MENU_H

#include <vector>
#include "GuiElement.h"
#include "GuiText.h"
#include "GuiComposite.h"

namespace Amju
{
class GuiMenu;
typedef RCPtr<GuiMenu> PGuiMenu;


class GuiMenuItem : public GuiText // ?????
{
public:
  // Size not specified - defaults to width of text when all displayed
  //  in one line
  GuiMenuItem(const std::string& text);
  GuiMenuItem(const std::string& text, CommandFunc commandFunc);
  GuiMenuItem(const std::string& text, PGuiCommand commandFunc);

  virtual void Draw();

protected:
  void Init(const std::string&);
};
typedef RCPtr<GuiMenuItem> PGuiMenuItem;


class GuiNestMenuItem : public GuiMenuItem
{
public:
  GuiNestMenuItem(const std::string& text, GuiMenu* childMenu);

  virtual void Draw();
  GuiMenu* GetChild() { return m_childMenu; }

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

  virtual void Draw();  

  virtual void AddChild(GuiElement* pItem);
  //void Clear();
  // TODO Add Insert, Remove items: call GuiComposite funcs

  // Adjust selected item when cursor moves
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

  // Set callback for when mouse is clicked away from menu
  void SetOnClickedAwayFunc(CommandFunc);

protected:
//  typedef std::vector<PGuiMenuItem> Items;
//  Items m_items;
  int m_selected; // index in m_items or -1
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

