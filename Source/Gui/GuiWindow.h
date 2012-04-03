#ifndef GUI_WINDOW_H_INCLUDED
#define GUI_WINDOW_H_INCLUDED

#include "GuiElement.h"

namespace Amju
{
// Decorator, sets viewport so child is clipped if we draw outside of the boundary
class GuiWindow : public GuiElement
{
public:

  virtual void Draw();
  virtual bool Load(File*); // Load pos and size

  virtual GuiElement* GetElementByName(const std::string& name);

protected:
  PGuiElement* m_child;
};
}

#endif

