#ifndef GUI_WINDOW_H_INCLUDED
#define GUI_WINDOW_H_INCLUDED

#include "GuiComposite.h"

namespace Amju
{
// Decorator, sets viewport so child is clipped if we draw outside of the boundary
class GuiWindow : public GuiComposite
{
public:
  virtual void Draw();

protected:
};
}

#endif

