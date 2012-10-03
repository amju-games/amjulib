#ifndef GUI_FILE_DIALOG_H_INCLUDED
#define GUI_FILE_DIALOG_H_INCLUDED

#include "GuiDialog.h"

namespace Amju
{
class GuiFileDialog : public GuiDialog
{
public:
  static const char* NAME;

  virtual bool Load(File* f);

  void SetPathAndFile(const std::string& path);
  std::string GetPathAndFile();

  void OnListboxClick(const std::string& fullPathAndFilename);
  void OnPathChange();

private:
  // Called when new file selected, show contents in preview pane
  void ShowPreview();
};

GuiElement* CreateFileDialog();
}

#endif

