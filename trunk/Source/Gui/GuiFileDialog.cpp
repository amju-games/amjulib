#include <StringUtils.h>
#include "GuiFileDialog.h"
#include "GuiFileListBox.h"
#include "GuiTextEdit.h"
#include "GuiObjView.h"

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
  lastPath = CleanPath(fullPathAndFilename); 
  GuiTextEdit* text = dynamic_cast<GuiTextEdit*>(GetElementByName("fd-path-text"));
  Assert(text);
  text->SetText(lastPath);

  ShowPreview();

std::cout << "Listbox click: lastPath is now \"" << lastPath << "\"\n";
}

void GuiFileDialog::OnPathChange()
{
  GuiTextEdit* text = dynamic_cast<GuiTextEdit*>(GetElementByName("fd-path-text"));
  Assert(text);
  lastPath = CleanPath(text->GetText()); 

  GuiFileListBox* fb = dynamic_cast<GuiFileListBox*>(GetElementByName("fd-file-list-box"));
  Assert(fb);
  fb->SetDir(lastPath);

  ShowPreview();

std::cout << "Path change: lastPath is now \"" << lastPath << "\"\n";
}

void GuiFileDialog::ShowPreview()
{
  // TODO Look up preview type associated with extension
  if (GetFileExt(lastPath) == "obj")
  {
    // TODO Load mesh - don't use ResourceManager
    ObjMesh* mesh = LoadObjMesh(lastPath);
    if (mesh)
    {
      GuiObjView* gov = dynamic_cast<GuiObjView*>(GetElementByName("fd-obj-view"));
      Assert(gov);
      gov->SetObjMesh(mesh);
    }
  }
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
  text->SetOnChangeFunc(Amju::OnPathChange);

  return true;
}

void GuiFileDialog::SetPathAndFile(const std::string& path)
{
  GuiFileListBox* fb = dynamic_cast<GuiFileListBox*>(GetElementByName("fd-file-list-box"));
  Assert(fb);
  fb->SetDir(GetFilePath(path));
  // Set path text
  GuiTextEdit* text = dynamic_cast<GuiTextEdit*>(GetElementByName("fd-path-text"));
  Assert(text);
  text->SetText(path);
}

std::string GuiFileDialog::GetPathAndFile()
{
  GuiTextEdit* text = dynamic_cast<GuiTextEdit*>(GetElementByName("fd-path-text"));
  Assert(text);
  return text->GetText();
}

}

