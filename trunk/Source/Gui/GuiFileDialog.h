#ifndef GUI_FILE_DIALOG_H_INCLUDED
#define GUI_FILE_DIALOG_H_INCLUDED

#include "GuiDialog.h"

namespace Amju
{
class GuiFileDialog : public GuiDialog
{
public:
  static const char* NAME;
};

GuiElement* CreateFileDialog();
}

#endif

