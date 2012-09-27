#include "GuiFileDialog.h"

namespace Amju
{
const char* GuiFileDialog::NAME = "gui-file-dialog";

GuiElement* CreateFileDialog()
{
  GuiFileDialog* d = new GuiFileDialog;
  return d;
}
}

