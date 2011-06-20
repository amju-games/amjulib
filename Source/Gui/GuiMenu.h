#ifndef GUI_MENU_H
#define GUI_MENU_H

#include <vector>
#include "GuiElement.h"
#include "GuiText.h"

namespace Amju
{
class GuiMenu;
typedef RCPtr<GuiMenu> PGuiMenu;


class GuiMenuItem : public GuiText // ?????
{
public:
  // Size not specified - defaults to width of text when all displayed
  //  in one line
  GuiMenuItem(const std::string& text, CommandFunc commandFunc = 0);

  virtual void Draw();
};
typedef RCPtr<GuiMenuItem> PGuiMenuItem;


class GuiNestMenuItem : public GuiMenuItem
{
public:
  GuiNestMenuItem(const std::string& text, GuiMenu* childMenu);

  virtual void Draw();
  GuiMenu* GetChild() { return m_childMenu; }

private:
  PGuiMenu m_childMenu;
};


class GuiMenu : public GuiElement
{
public:
  GuiMenu();
  virtual ~GuiMenu();

  void SetIsVertical(bool isVertical) { m_isVertical = isVertical; } 
  bool IsVertical() const { return m_isVertical; }

  virtual void Draw();  

  void AddItem(GuiMenuItem* pItem);
  void Clear();
  // TODO Insert, Remove items

  // Adjust selected item when cursor moves
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

protected:
  typedef std::vector<PGuiMenuItem> Items;
  Items m_items;
  int m_selected; // index in m_items or -1
  Vec2f m_cursorPos; // last position passed into OnCursorEvent

  PGuiMenu m_childMenu; // ?
  bool m_isVertical;
};
}

#endif

