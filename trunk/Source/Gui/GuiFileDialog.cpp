#include "GuiFileDialog.h"
#include "GuiFileListBox.h"

namespace Amju
{
const char* GuiFileDialog::NAME = "gui-file-dialog";

GuiElement* CreateFileDialog()
{
  GuiFileDialog* d = new GuiFileDialog;
  return d;
}

bool GuiFileDialog::Load(File* f)
{
  if (!GuiDialog::Load(f))
  {
    return false;
  }

  // Element name is in text file.
  // TODO Look up by type instead ??
  GuiFileListBox* fb = dynamic_cast<GuiFileListBox*>(GetElementByName("fd-file-list-box"));
  Assert(fb);

  // Set default/start dir
#ifdef WIN32
  fb->SetDir("c:\\");
#endif

#ifdef MACOSX
  fb->SetDir("/");
#endif

  return true;
}

}

