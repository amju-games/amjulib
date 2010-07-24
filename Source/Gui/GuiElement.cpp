#include "GuiElement.h"
#include "GuiFactory.h"
#include "StringUtils.h"
#include "EventPoller.h"
#include "LoadVec2.h"

namespace Amju
{
Rect GetRect(GuiElement* elem)
{
  return Rect(
    elem->GetPos().x, 
    elem->GetPos().x + elem->GetSize().x, 
    elem->GetPos().y - elem->GetSize().y,
    elem->GetPos().y);
}

GuiElement::GuiElement()
{
  m_isVisible = true;
}

PGuiElement LoadGui(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    return 0;
  }
  std::string s;
  if (!f.GetDataLine(&s))
  {
    f.ReportError("Expected gui element type");
    return 0;
  }
  PGuiElement gui = TheGuiFactory::Instance()->Create(s);
  if (!gui)
  {
    f.ReportError("Failed to create gui element; type: " + s);
    return 0;
  }
  if (!gui->Load(&f))
  {
    return 0;
  }
  return gui;
}

GuiElement::~GuiElement()
{
  // TODO Subclasses should remove themselves - just make sure we are deleted here
  //TheEventPoller::Instance()->RemoveListener(this); 
  Assert(!TheEventPoller::Instance()->HasListener(this));
}

bool GuiElement::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    Assert(0);
    return false;
  }

  if (!LoadVec2(f, &m_pos))
  {
    Assert(0);
    return false;
  }

  if (!LoadVec2(f, &m_size))
  {
    Assert(0);
    return false;
  }

  return true;
}

bool GuiElement::OpenAndLoad(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    Assert(0);
    return false;
  }
  return Load(&f);
}

void GuiElement::SetCommand(PGuiCommand pCommand)
{
  m_pCommand = pCommand;
}

void GuiElement::ExecuteCommand()
{
  TheGuiCommandHandler::Instance()->DoNewCommand(m_pCommand);
}

GuiElement* GuiElement::GetElementByName(const std::string& name)
{
  if (name == m_name)
  {
    return this;
  }
  return 0;
}

void GuiElement::SetPos(const Vec2f& v)
{
  m_pos = v;
}

const Vec2f& GuiElement::GetPos() const
{
  return m_pos;
}

void GuiElement::SetSize(const Vec2f& v)
{
  m_size = v;
}

const Vec2f& GuiElement::GetSize() const
{
  return m_size;
}

void GuiElement::SetVisible(bool isVis)
{
  m_isVisible = isVis;
}

bool GuiElement::IsVisible() const
{
  return m_isVisible;
}
}
