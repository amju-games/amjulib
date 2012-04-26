#include "GuiElement.h"
#include "GuiFactory.h"
#include "StringUtils.h"
#include "EventPoller.h"
#include "LoadVec2.h"

namespace Amju
{
static GuiElement* focusElement = 0;
static float globalScale = 1.0f;
static bool textToSpeechEnabled = true;

void GuiElement::SetHasFocus(bool f)
{
  if (f)
  {
    focusElement = this;
    OnGetFocus();

    if (IsTextToSpeechEnabled())
    {
      TextToSpeech();
    }
  }
  else
  {
    focusElement = 0;
  }
}

bool GuiElement::HasFocus() const
{
  return (focusElement == this);
}

void GuiElement::SetGlobalScale(float f)
{
  globalScale = f;
}

float GuiElement::GetGlobalScale()
{
  return globalScale;
}

void GuiElement::SetTextToSpeechEnabled(bool t)
{
  textToSpeechEnabled = t;
}

bool GuiElement::IsTextToSpeechEnabled()
{
  return textToSpeechEnabled;
}

Rect GetRect(GuiElement* elem)
{
  return Rect(
    elem->GetPos().x, 
    elem->GetPos().x + elem->GetSize().x, 
    elem->GetPos().y - elem->GetSize().y,
    elem->GetPos().y);
}

GuiElement* GetElementByName(GuiElement* root, const std::string& nodeName)
{
  GuiElement* elem = root->GetElementByName(nodeName);
  if (!elem)
  {
std::cout << "Unexpected: no node named \"" << nodeName << "\" in GUI tree with root \"" << root->GetName() << "\"\n";
    Assert(0);
  }
  return elem;
}

GuiElement::GuiElement()
{
  m_parent = 0;
  m_isVisible = true;
  m_isSelected = false;
  m_commandFunc = 0;
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
  //Assert(!TheEventPoller::Instance()->HasListener(this));
  // removed in EventListener dtor, so not removed yet.
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
  Assert(!m_commandFunc);
  m_pCommand = pCommand;
}

void GuiElement::SetCommand(CommandFunc commandFunc)
{
  Assert(!m_pCommand);
  m_commandFunc = commandFunc;
}

void GuiElement::SetName(const std::string& name)
{
  m_name = name;
}

const std::string& GuiElement::GetName() const
{
  return m_name;
}

void GuiElement::ExecuteCommand()
{
  if (m_commandFunc)
  {
    Assert(!m_pCommand);
    m_commandFunc();
  }
  else
  {
    TheGuiCommandHandler::Instance()->DoNewCommand(m_pCommand);
  }
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

Vec2f GuiElement::GetSize() const
{
  return m_size * GetGlobalScale();
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
