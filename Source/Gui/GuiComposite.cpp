#include "GuiComposite.h"
#include "GuiFactory.h"

namespace Amju
{
const char* GuiComposite::NAME = "gui-comp";

void GuiComposite::Draw()
{
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    m_children[i]->Draw();
  }
}

GuiElement* GuiComposite::GetElementByName(const std::string& name)
{
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    GuiElement* e = m_children[i]->GetElementByName(name);
    if (e)
    {
      return e;
    }
  }
  return 0;
}

bool GuiComposite::Load(File* f)
{
  int num = 0;
  if (!f->GetInteger(&num))
  {
    Assert(0);
    return false;
  }
  m_children.reserve(num);
  for (int i = 0; i < num; i++)
  {
    std::string s;
    if (!f->GetDataLine(&s))
    {
      Assert(0);
      return false;
    }
    PGuiElement e = TheGuiFactory::Instance()->Create(s);
    Assert(e);
    if (!e->Load(f))
    {
      return false;
    }
    m_children.push_back(e);
  }
  return true;
}
}
