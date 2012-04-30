#ifndef GUI_KEYBOARD_H_INCLUDED
#define GUI_KEYBOARD_H_INCLUDED

#include "GuiWindow.h"

namespace Amju
{
// Generates keyboard events
class GuiKeyboard : public GuiWindow
{
public:
  static const char* NAME;
  virtual bool Load(File*);

protected:
};
}

#endif

