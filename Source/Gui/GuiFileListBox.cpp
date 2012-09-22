#include <Directory.h>
#include "GuiFileListBox.h"
#include "GuiScroll.h"

namespace Amju
{
const char* GuiFileListBox::NAME = "gui-file-list-box";

GuiElement* CreateFileListBox()
{
  GuiFileListBox* w = new GuiFileListBox;
  GuiScroll* s = new GuiScroll;
  GuiList* lb = new GuiList;
  w->AddChild(s);
  s->AddChild(lb);
  return w; 
}

GuiFileListBox::GuiFileListBox()
{
}

void GuiFileListBox::SetDir(const std::string& dir)
{
  m_dir = dir;
  Refresh();
}

void GuiFileListBox::Refresh()
{
  Clear();
  
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
std::cout << "Done!\n";
}
}


