#ifndef GUI_FILE_LIST_BOX_H_INCLUDED
#define GUI_FILE_LIST_BOX_H_INCLUDED

#include "GuiListBox.h"

namespace Amju
{
class GuiFileListBox : public GuiListBox
{
protected:
  GuiFileListBox();
  friend GuiElement* CreateFileListBox();

public:
  static const char* NAME;

  void SetDir(const std::string& dir);
  void Refresh();

protected:
  std::string m_dir;
};

GuiElement* CreateFileListBox();
}

#endif

