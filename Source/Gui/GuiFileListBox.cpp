#include "GuiFileListBox.h"
#include "GuiScroll.h"

namespace Amju
{
const char* GuiFileListBox::NAME = "gui-file-list-box";

void DoubleClickFile(GuiList* lb, int selectedIndex)
{
  GuiElement* elem = lb->GetParent()->GetParent();
  Assert(elem);
  Assert(dynamic_cast<GuiFileListBox*>(elem));
  GuiFileListBox* flb = (GuiFileListBox*)elem;
  flb->OnDoubleClick(selectedIndex);
}

GuiElement* CreateFileListBox()
{
  GuiFileListBox* w = new GuiFileListBox;
  GuiScroll* s = new GuiScroll;
  GuiList* lb = new GuiList;
  w->AddChild(s);
  s->AddChild(lb);

  lb->SetDoubleClickFunc(DoubleClickFile);
  lb->SetIsMultiSel(false);

  return w; 
}

GuiFileListBox::GuiFileListBox()
{
  m_filechooseCallback = 0;
}

void GuiFileListBox::SetFileChooseCallback(FileChooseCallback cb)
{
  m_filechooseCallback = cb;
}

void GuiFileListBox::OnDoubleClick(int selectedIndex)
{
  Assert(selectedIndex >= 0);
  Assert(selectedIndex < (int)m_dirents.size());
  DirEnt& de = m_dirents[selectedIndex];
  if (de.m_isDir)
  {
    std::string dir = m_dir + "/" + de.m_name;
    SetDir(dir);
  }
  else
  {
std::cout << "Executing command for " << de.m_name << "\n";

    // Much more useful to call a callback with the full path and filename
    if (m_filechooseCallback)
    {
      std::string f = m_dir + "/" + de.m_name;
      m_filechooseCallback(f);
    }
    else
    {
      // No file choose callback, so try to execute command for selected element
      GuiElement* elem = GetList()->GetChild(selectedIndex);
      elem->ExecuteCommand();
    }
  }
}

void GuiFileListBox::SetDir(const std::string& dir)
{
  m_dir = dir;
  Refresh();
  GetScroll()->Reset();
}

void GuiFileListBox::Refresh()
{
  Clear();
  
  m_dirents.clear();
  DirEnts des;
  Dir(m_dir, &des, false);

std::cout << "GuiFileListBox: populating list, dir: " << m_dir << "\n";

  for (unsigned int i = 0; i < des.size(); i++)
  {
    DirEnt& de = des[i];
    // TODO Show difference between files and dirs
    // if (!de.m_isDir) ...

//std::cout << " ..found " << de.m_name << "\n";

    if (de.m_name == ".")
    {
      continue;
    }

    // TODO flag for hidden dirs
    if (de.m_name[0] == '.' && de.m_name != "..")
    {
      continue;
    }

    m_dirents.push_back(de);

    GuiText* elem = new GuiText;
    std::string name = de.m_name;
    if (de.m_isDir)
    {
      name = "<DIR> " + name;
      //elem->SetScaleX(0.5f); // TODO Some good way of showing difference
    }
    elem->SetText(name);
    GetList()->AddItem(elem);
  }
//std::cout << "Done!\n";
}
}


