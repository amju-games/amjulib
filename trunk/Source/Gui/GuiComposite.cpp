#include <EventPoller.h>
#include "GuiComposite.h"
#include "GuiFactory.h"

namespace Amju
{
const char* GuiComposite::NAME = "gui-comp";

GuiComposite::GuiComposite()
{
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
  if (name == GetName())
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
  std::string name;
  if (!f->GetDataLine(&name))
  {
    f->ReportError("Gui comp: expected name");
    Assert(0);
    return false;
  }
  SetName(name);

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
    e->SetParent(this);
    if (!e->Load(f))
    {
      return false;
    }

    AddChild(e); // subclasses can override this and resize etc
  }
  return true;
}

bool GuiComposite::OnCursorEvent(const CursorEvent& e)
{
  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnCursorEvent(e))
    {
      ret = true;
    }
  }
  return ret;
}

bool GuiComposite::OnRotationEvent(const RotationEvent& e)
{
  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnRotationEvent(e))
    {
      ret = true;
    }
  }
  return ret;
}

bool GuiComposite::OnJoyAxisEvent(const JoyAxisEvent& e)
{
  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnJoyAxisEvent(e))
    {
      ret = true;
    }
  }
  return ret;
}

bool GuiComposite::OnButtonEvent(const ButtonEvent& e)
{
  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnButtonEvent(e))
    {
      ret = true;
    }
  }
  return ret;
}

bool GuiComposite::OnMouseButtonEvent(const MouseButtonEvent& e)
{
  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnMouseButtonEvent(e))
    {
      ret = true;
    }
  }
  return ret;
}

bool GuiComposite::OnKeyEvent(const KeyEvent& ke)
{
  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnKeyEvent(ke))
    {
      ret = true;
    }
  }
  if (ret)
  {
    return true;
  }

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
  int s = m_children.size();
  for (int i = 0; i < s; i++)
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
