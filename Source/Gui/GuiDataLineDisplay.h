#ifndef GUI_DATA_LINE_DISPLAY_H_INCLUDED
#define GUI_DATA_LINE_DISPLAY_H_INCLUDED

#include "GuiDataDisplay.h"

namespace Amju
{
class GuiDataLineDisplay : public GuiDataDisplay
{
public:
  static const char* NAME;  
  virtual void Draw();
  virtual bool Load(File*);

  virtual bool OnCursorEvent(const CursorEvent&);
};

GuiElement* CreateDataLineDisplay();
}

#endif

