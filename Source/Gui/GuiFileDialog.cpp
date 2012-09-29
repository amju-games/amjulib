#include "GuiFileDialog.h"
#include "GuiFileListBox.h"
#include "GuiTextEdit.h"

namespace Amju
{
const char* GuiFileDialog::NAME = "gui-file-dialog";

// Only one is expected to be active at a time.... Hmm...
static GuiFileDialog* lastFileDialog = 0;

static std::string lastPath;

GuiElement* CreateFileDialog()
{
  GuiFileDialog* d = new GuiFileDialog;
  return d;
}

void OnListboxClick(const std::string& fullPathAndFilename)
{
  Assert(lastFileDialog);
  lastFileDialog->OnListboxClick(fullPathAndFilename);
}

void OnPathChange()
{
  Assert(lastFileDialog);
  lastFileDialog->OnPathChange();
}

void GuiFileDialog::OnListboxClick(const std::string& fullPathAndFilename)
{
  lastPath = fullPathAndFilename; 
  GuiTextEdit* text = dynamic_cast<GuiTextEdit*>(GetElementByName("fd-path-text"));
  Assert(text);
  text->SetText(fullPathAndFilename);
}

void GuiFileDialog::OnPathChange()
{
  GuiTextEdit* text = dynamic_cast<GuiTextEdit*>(GetElementByName("fd-path-text"));
  Assert(text);
  lastPath = text->GetText(); 

  GuiFileListBox* fb = dynamic_cast<GuiFileListBox*>(GetElementByName("fd-file-list-box"));
  Assert(fb);
  fb->SetDir(lastPath);
}

bool GuiFileDialog::Load(File* f)
{
  if (!GuiDialog::Load(f))
  {
    return false;
  }

  lastFileDialog = this;

  // Element name is in text file.
  // TODO Look up by type instead ??
  GuiFileListBox* fb = dynamic_cast<GuiFileListBox*>(GetElementByName("fd-file-list-box"));
  Assert(fb);
  fb->SetDoubleClickCallback(Amju::OnListboxClick);
  fb->SetSingleClickCallback(Amju::OnListboxClick);

  std::string path = lastPath; 
  // Set default/start dir
  if (path.empty())
  {
#ifdef WIN32
    path = "c:\\";
#endif

#ifdef MACOSX
    path = "/";
#endif
  }

  fb->SetDir(path);
  // Set path text
  GuiTextEdit* text = dynamic_cast<GuiTextEdit*>(GetElementByName("fd-path-text"));
  Assert(text);
  text->SetText(path);

  return true;
}

}

