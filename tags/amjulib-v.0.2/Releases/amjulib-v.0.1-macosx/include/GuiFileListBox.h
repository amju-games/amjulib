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

  typedef void (*ClickCallback)(const std::string& fullPathAndFilename);
  void SetDoubleClickCallback(ClickCallback);
  void SetSingleClickCallback(ClickCallback);

protected:
  friend void DoubleClickFile(GuiList*, int);
  void OnDoubleClick(int selectedIndex);

  friend void SingleClickFile(GuiList*, int);
  void OnSingleClick(int selectedIndex);

protected:
  std::string m_dir;
  DirEnts m_dirents;
  ClickCallback m_doubleClickCallback;
  ClickCallback m_singleClickCallback;
};

GuiElement* CreateFileListBox();
}

#endif

