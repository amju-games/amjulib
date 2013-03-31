#include <AmjuFirst.h>
#include "GuiElement.h"
#include "GuiFactory.h"
#include "StringUtils.h"
#include "EventPoller.h"
#include "LoadVec2.h"
#include <AmjuFinal.h>

#define ENABLE_FOCUS_ELEMENT

namespace Amju
{
static RCPtr<GuiElement> focusElement = 0;
static float globalScale = 1.0f;
static bool textToSpeechEnabled = true;

void GuiElement::SetHasFocus(bool f)
{
#ifdef ENABLE_FOCUS_ELEMENT

  if (!IsVisible())
  {
std::cout << "GUI: setting focus to " << m_name << " but not visible, so no effect.\n";
    return;
  }

  if (f)
  {
#ifdef FOCUS_DEBUG
std::cout << "GUI: setting focus to " << m_name;
if (focusElement == this)
{
  std::cout << "(already had focus)\n";
}
else if (focusElement)
{
  std::cout << " (" << focusElement->m_name << " loses focus)\n";
}
else
{
  std::cout << " (no previous focus)\n";
}
#endif

    focusElement = this;
    OnGetFocus();
    if (m_onFocusFunc)
    {
      m_onFocusFunc();
    }

    if (IsTextToSpeechEnabled())
    {
      TextToSpeech();
    }
  }
  else
  {
#ifdef FOCUS_DEBUG
std::cout << "GUI: REMOVING focus from " << m_name;
if (focusElement == this)
{
  std::cout << "(did have focus)\n";
}
else if (focusElement)
{
  std::cout << " (" << focusElement->m_name << " would loses focus, disregarding)\n";
}
else
{
  std::cout << " (no previous focus)\n";
}
#endif

    if (focusElement == this)
    {
      focusElement = 0;
    }
  }
#endif // ENABLE_FOCUS_ELEMENT
}

bool GuiElement::HasFocus() const
{
  return (focusElement == this);
}

void GuiElement::SetOnFocusFunc(CommandFunc f)
{
  m_onFocusFunc = f;
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
  Vec2f pos = elem->GetCombinedPos();
  Vec2f size = elem->GetSize();

  return Rect(pos.x, pos.x + size.x, pos.y - size.y, pos.y);
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
  m_onFocusFunc = 0;
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
  TheEventPoller::Instance()->AddListener(gui);
  return gui;
}

GuiElement::~GuiElement()
{
  // TODO Subclasses should remove themselves - just make sure we are deleted here
  //TheEventPoller::Instance()->RemoveListener(this); 
  //Assert(!TheEventPoller::Instance()->HasListener(this));
  // removed in EventListener dtor, so not removed yet.

//  std::cout << "Deleting GuiElement " << m_name << "\n";
}

bool GuiElement::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Gui element: expected name");
    Assert(0);
    return false;
  }

  if (!LoadVec2(f, &m_localpos))
  {
    Assert(0);
    return false;
  }

  if (!LoadVec2(f, &m_size))
  {
    Assert(0);
    return false;
  }

  Assert(m_size.x > 0);
  Assert(m_size.y > 0);

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

void GuiElement::SetLocalPos(const Vec2f& v)
{
  m_localpos = v;
}

Vec2f GuiElement::GetLocalPos() const
{
  return m_localpos * GetGlobalScale();
}

Vec2f GuiElement::GetCombinedPos() const
{
  GuiElement* parent = (const_cast<GuiElement*>(this))->GetParent();

  if (parent)
  {
    return GetLocalPos() + parent->GetCombinedPos(); 
  }
  else
  {
    return GetLocalPos();
  }
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
  if (!m_isVisible)
  {
    return false;
  }

  if (const_cast<GuiElement*>(this)->GetParent())
  {
    return (const_cast<GuiElement*>(this))->GetParent()->IsVisible();
  }

  return true;
}
}
