#ifndef GUI_FILE_LIST_BOX_H_INCLUDED
#define GUI_FILE_LIST_BOX_H_INCLUDED

#include "GuiListBox.h"

namespace Amju
{
class GuiFileListBox : public GuiListBox
{
public:
  static const char* NAME;

  GuiFileListBox();
  void SetDir(const std::string& dir);
  void Refresh();

protected:
  std::string m_dir;
};
}

#endif

