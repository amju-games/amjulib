#ifndef GUI_DIALOG_H_INCLUDED
#define GUI_DIALOG_H_INCLUDED

#include "GuiWindow.h"

namespace Amju
{
class GuiDialog : public GuiWindow
{
public:
  static const char* NAME;
 
  virtual void Draw();
  virtual bool Load(File*);
};
}

#endif

