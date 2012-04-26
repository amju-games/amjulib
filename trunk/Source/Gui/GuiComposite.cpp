#include <EventPoller.h>
#include "GuiComposite.h"
#include "GuiFactory.h"

namespace Amju
{
const char* GuiComposite::NAME = "gui-comp";

static const int COMP_PRIORITY = -100; // takes priority over most stuff ???

GuiComposite::GuiComposite()
{
  TheEventPoller::Instance()->AddListener(this, COMP_PRIORITY); 
}

int GuiComposite::GetNumChildren() const
{
  return m_children.size();
}

void GuiComposite::Clear()
{
  m_children.clear();
}

GuiElement* GuiComposite::GetChild(int i)
{
  Assert(i < GetNumChildren());
  return m_children[(unsigned int)i];
}

void GuiComposite::AddChild(GuiElement* elem)
{
  elem->SetParent(this);
  m_children.push_back(elem);
}

void GuiComposite::Draw()
{
  if (!IsVisible())
  {
    return;
  }

  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    m_children[i]->Draw();
  }
}

GuiElement* GuiComposite::GetElementByName(const std::string& name)
{
  if (name == m_name)
  {
    return this;
  }

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
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Gui comp: expected name");
    Assert(0);
    return false;
  }

  // No pos and size, so not using base class impl
  return LoadChildren(f);
}

bool GuiComposite::LoadChildren(File* f)
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
    if (!e)
    {
      f->ReportError("Failed to create GUI element of type " + s);
      return false;
    }
    Assert(e);
    if (!e->Load(f))
    {
      return false;
    }
    e->SetParent(this);

    m_children.push_back(e);
  }
  return true;
}

bool GuiComposite::OnKeyEvent(const KeyEvent& ke)
{
  if (!IsVisible())
  {
    return false;
  }

  if (!ke.keyDown)
  {
    return false;
  }

  if (ke.keyType == AMJU_KEY_DOWN)
  {
    return SetFocusNextChild();
  }
  else if (ke.keyType == AMJU_KEY_UP)
  {
    return SetFocusPrevChild();
  }
  return false;
}

bool GuiComposite::SetFocusPrevChild()
{
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    if (m_children[i]->HasFocus())
    {
      int next = i - 1; 
      if (next == -1)
      {
        next = m_children.size() - 1;
      }
      m_children[next]->SetHasFocus(true);
      return true;
    }
  }
  return false;
}

bool GuiComposite::SetFocusNextChild()
{
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    if (m_children[i]->HasFocus())
    {
      unsigned int next = i + 1; 
      if (next == m_children.size())
      {
        next = 0;
      }
      m_children[next]->SetHasFocus(true);
      return true;
    }
  }
  return false;
}

}
