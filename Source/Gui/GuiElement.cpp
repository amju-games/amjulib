// Amjulib - cross platform game engine
// (c) Copyright Jason Colman 2000-2017

#include <AmjuFirst.h>
#include <Colour.h>
#include <DrawBorder.h>
#include <EventPoller.h>
#include <GuiElement.h>
#include <GuiFactory.h>
#include <LoadVec2.h>
#include <Screen.h>
#include <StringUtils.h>
#include <AmjuFinal.h>

#define ENABLE_FOCUS_ELEMENT

namespace Amju
{
static RCPtr<GuiElement> focusElement = 0;
static float globalScale = 1.0f;
static bool textToSpeechEnabled = true;

GuiElement::GuiElement()
{
  m_parent = nullptr;
  m_isVisible = true;
  m_isSelected = false;
  m_commandFunc = nullptr;
  m_onFocusFunc = nullptr;
  m_drawBorder = false;
  m_userData = nullptr;
  m_ancestorsAreVisible = true;
}

GuiElement::~GuiElement()
{
}

void GuiElement::Draw()
{
  if (IsVisible() && m_drawBorder)
  {
    DrawBorder(this, Colour(1, 1, 1, 1));
  }
}

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
      m_onFocusFunc(this);
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

PGuiElement LoadGui(const std::string& filename, bool addAsListener)
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
    f.ReportError("Failed to load gui element; type: " + s);
    return 0;
  }
   
  if (addAsListener)
  {
    TheEventPoller::Instance()->AddListener(gui);
  }

  return gui;
}

bool GuiElement::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Gui element: expected name");
    Assert(0);
    return false;
  }

  Vec2f pos;
  if (!LoadVec2(f, &pos))
  {
    f->ReportError("Gui element: failed to load pos");
    Assert(0);
    return false;
  }
  SetLocalPos(pos);

  Vec2f size;
  if (!LoadVec2(f, &size))
  {
    f->ReportError("Gui element: failed to load size");
    Assert(0);
    return false;
  }
  SetSize(size);

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
    m_commandFunc(this);
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
  SetAncestorsVisible(isVis);
}

void GuiElement::SetAncestorsVisible(bool ancestorVis)
{
  m_ancestorsAreVisible = ancestorVis;
}
  
bool GuiElement::IsVisible() const
{
  return m_isVisible && m_ancestorsAreVisible;
}
}
