#ifndef GUI_MENU_H
#define GUI_MENU_H

#include <vector>
#include "GuiElement.h"
#include "GuiText.h"

namespace Amju
{
class GuiMenuItem : public GuiText // ?????
{
public:
  // Size not specified - defaults to width of text when all displayed
  //  in one line
  GuiMenuItem(const std::string& text);

  // Specify size, which is the bounding box for the text. This is used
  //  for right/centre justification
  GuiMenuItem(const std::string& text, const Vec2f& size);

  virtual void Draw();
};
typedef RCPtr<GuiMenuItem> PGuiMenuItem;

class GuiMenu : public GuiElement
{
public:
  GuiMenu();
  virtual void Draw();  

  void AddItem(GuiMenuItem* pItem);
  // TODO Insert, Remove items

  // Adjust selected item when cursor moves
  virtual void OnCursorEvent(const CursorEvent&);

protected:
  typedef std::vector<PGuiMenuItem> Items;
  Items m_items;
  int m_selected; // index in m_items or -1
};
}

#endif

