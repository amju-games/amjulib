#ifndef GUI_FILE_LIST_BOX_H_INCLUDED
#define GUI_FILE_LIST_BOX_H_INCLUDED

#include <Directory.h>
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

  typedef void (*FileChooseCallback)(const std::string& fullPathAndFilename);
  void SetFileChooseCallback(FileChooseCallback);

protected:
  friend void DoubleClickFile(GuiList*, int);
  void OnDoubleClick(int selectedIndex);

protected:
  std::string m_dir;
  DirEnts m_dirents;
  FileChooseCallback m_filechooseCallback;
};

GuiElement* CreateFileListBox();
}

#endif

