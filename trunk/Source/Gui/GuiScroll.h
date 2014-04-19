#ifndef GUI_SCROLL_H_INCLUDED
#define GUI_SCROLL_H_INCLUDED

#include "GuiComposite.h"
#include "GuiScrollBar.h"

namespace Amju
{
class GuiScroll : public GuiComposite
{
public:
  static const char* NAME;

  virtual bool Load(File*); 
  virtual void Draw();
  virtual void Update();

  void Reset(); // Reset offset position and vel

  // Scroll events: could be generated by a scroll bar, keyboard, etc.
  // TODO Make scroll events a type of Event ??
  void OnScrollVelEvent(const Vec2f& scrollVel);

  virtual bool OnKeyEvent(const KeyEvent&); 
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

  void InitScrollBar();

private:
  // Offset in x and y
  Vec2f m_scrollPos;
  Vec2f m_scrollVel; 

  RCPtr<GuiScrollBar> m_scrollBar;
};
}

#endif

