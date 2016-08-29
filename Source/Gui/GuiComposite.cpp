#include <AmjuFirst.h>
#include <EventPoller.h>
#include "GuiComposite.h"
#include "GuiFactory.h"
#include <AmjuFinal.h>

namespace Amju
{
// In a single GUI composite, all descendents have equal priority. 
// If this flag is true, stop propagating an event once handled.
static const bool s_stopWhenEventHandled = true; // TODO

const char* GuiComposite::NAME = "gui-comp";

GuiComposite::GuiComposite()
{
}

GuiComposite::~GuiComposite()
{
  Clear(); // to find crash
}

void GuiComposite::SetSizeFromChildren()
{
  Vec2f size;
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    GuiElement* child = m_children[i];
    Vec2f chSize = child->GetSize();
    Vec2f chPos = child->GetCombinedPos();
    chPos.x = fabs(chPos.x); // any offset increases size?? Is this right?? TODO
    chPos.y = fabs(chPos.y);
    chSize += chPos;

    size.x = std::max(size.x, chSize.x);
    size.y = std::max(size.y, chSize.y);
  }
  SetSize(size);
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

void GuiComposite::SetVisible(bool isVis)
{
  GuiElement::SetVisible(isVis);
  for (auto ch : m_children)
  {
    ch->SetVisible(isVis);
  }
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
    f->ReportError("Expected num children for GUI composite.");
    Assert(0);
    return false;
  }
  m_children.reserve(num);
  for (int i = 0; i < num; i++)
  {
    if (!LoadOneChild(f))
    {
      return false;
    }
  }
  return true;
}

bool GuiComposite::LoadOneChild(File* f)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("GUI composite: LoadOneChild: Expected more data, but have reached end of file.");
    Assert(0);
    return false;
  }
  PGuiElement e = TheGuiFactory::Instance()->Create(s);
  if (!e)
  {
    f->ReportError("Failed to create GUI element of type " + s);
    Assert(0);
    return false;
  }
  Assert(e);
  e->SetParent(this);
  if (!e->Load(f))
  {
    f->ReportError("Failed to load child of GUI composite.");
    return false;
  }

  AddChild(e); // subclasses can override this and resize etc
 
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
      if (s_stopWhenEventHandled)
      {
        break; 
      }
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
      if (s_stopWhenEventHandled)
      {
        break; 
      }
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
      if (s_stopWhenEventHandled)
      {
        break; 
      }
    }
  }
  if (ret)
  {
    return true;
  }

  // TODO move to Gui Menu
  static bool up = false; // TODO member var
  static bool down = false;
  if (e.y > 0.5f) // TODO Sensitivity
  {
    if (!up)
    {
      SetFocusPrevChild();
    }
    up = true;
  }
  else
  {
    up = false;
  }

  if (e.y < -0.5f)
  {
    if (!down)
    {
      SetFocusNextChild();
    }
    down = true;
  }
  else
  {
    down = false;
  }

  return true;
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
      if (s_stopWhenEventHandled)
      {
        break; 
      }
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
      if (s_stopWhenEventHandled)
      {
        break; 
      }
    }
  }
  return ret;
}

bool GuiComposite::OnDoubleClickEvent(const DoubleClickEvent& e)
{
  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnDoubleClickEvent(e))
    {
      ret = true;
      if (s_stopWhenEventHandled)
      {
        break; 
      }
    }
  }
  return ret;
}

bool GuiComposite::OnBalanceBoardEvent(const BalanceBoardEvent& e)
{
  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnBalanceBoardEvent(e))
    {
      ret = true;
      if (s_stopWhenEventHandled)
      {
        break; 
      }
    }
  }
  return ret;
}

bool GuiComposite::OnTextEvent(const TextEvent& e)
{
  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnTextEvent(e))
    {
      ret = true;
      if (s_stopWhenEventHandled)
      {
        break; 
      }
    }
  }
  return ret;
}

bool GuiComposite::OnQuitEvent()
{
  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnQuitEvent())
    {
      ret = true;
      if (s_stopWhenEventHandled)
      {
        break; 
      }
    }
  }
  return ret;
}

bool GuiComposite::OnResizeEvent(const ResizeEvent& e)
{
  bool ret = false;
  int s = m_children.size();
  for (int i = 0; i < s; i++)
  {
    if (m_children[i]->OnResizeEvent(e))
    {
      ret = true;
      if (s_stopWhenEventHandled)
      {
        break; 
      }
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
      // In a single GUI, all descendents have equal priority. 
      // Stop when event first handled.
      // Maybe this should be a flag, as perhaps sometimes we want all
      //  descendents to get the event?
      if (s_stopWhenEventHandled)
      {
        break; 
      }
    }
  }
  if (ret)
  {
    return true;
  }

  // TODO Move to GuiMenu ?

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
