#ifndef GUI_WINDOW_H_INCLUDED
#define GUI_WINDOW_H_INCLUDED

#include "GuiComposite.h"

namespace Amju
{
// Decorator, sets viewport so child is clipped if we draw outside of the boundary
class GuiWindow : public GuiComposite
{
public:
  static const char* NAME;

  virtual void Draw();
  virtual bool Load(File*); 

  // Discard if outside this Window's region, right ?
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  virtual bool OnDoubleClickEvent(const DoubleClickEvent&);
  virtual bool OnCursorEvent(const CursorEvent&);

protected:
  // Our rectangle after clipping by any ancestor Windows
  Rect m_clippedRect;
};
}

#endif

