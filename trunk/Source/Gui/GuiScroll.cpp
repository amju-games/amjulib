#include <algorithm>
#include <AmjuGL.h>
#include <Timer.h>
#include "GuiScroll.h"
#include "GuiFactory.h"

namespace Amju
{
const char* GuiScroll::NAME = "gui-scroll";

void GuiScroll::Reset()
{
  m_scrollPos = Vec2f(0, 0);
  m_scrollVel = Vec2f(0, 0);
}

bool GuiScroll::OnKeyEvent(const KeyEvent& e)
{
  // TODO has focus ?

  if (!IsVisible())
  {
    return false;
  }

  // Respond on key down event, right..?
  if (e.keyType == AMJU_KEY_UP && e.keyDown)
  {
    OnScrollVelEvent(Vec2f(0, -1.0f));
    return true;
  }
  else if (e.keyType == AMJU_KEY_DOWN && e.keyDown)
  {
    OnScrollVelEvent(Vec2f(0, 1.0f));
    return true;
  }
  if (e.keyType == AMJU_KEY_LEFT && e.keyDown)
  {
    OnScrollVelEvent(Vec2f(-1.0f, 0));
    return true;
  }
  else if (e.keyType == AMJU_KEY_RIGHT && e.keyDown)
  {
    OnScrollVelEvent(Vec2f(1.0f, 0));
    return true;
  }
  return false;
}

static bool leftDrag = false;

bool GuiScroll::OnCursorEvent(const CursorEvent& ce)
{
  Assert(m_children.size() == 1);
  GuiElement* child = m_children[0];

  // TODO put dx/dy in Cursor Event
  static float oldx = ce.x;
  static float oldy = ce.y;
  float dx = ce.x - oldx;
  float dy = ce.y - oldy;
  oldx = ce.x;
  oldy = ce.y;

  if (leftDrag)
  {
    OnScrollVelEvent(Vec2f(dx * 10.0f, dy * 10.0f)); // TODO TEMP TEST x/y scroll flags
  }

  return child->OnCursorEvent(ce); 
}

bool GuiScroll::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  Assert(m_children.size() == 1);
  GuiElement* child = m_children[0];

  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    leftDrag = mbe.isDown;
  }

  return child->OnMouseButtonEvent(mbe); 
}

void GuiScroll::Draw()
{
  Assert(m_children.size() == 1);
  GuiElement* child = m_children[0];

  // Update not called. TODO
  float dt = TheTimer::Instance()->GetDt();
  static const float DECEL = 0.5f; // TODO
  m_scrollVel *= std::max(0.0f, (1.0f - DECEL * dt));
  m_scrollPos += m_scrollVel * dt;

  // TODO x-axis
  // Bounce or stop on end reached
  if (m_scrollPos.x < 0)
  {
    m_scrollPos.x = 0;
#ifdef BOUNCE
    m_scrollVel.x = -0.25f * m_scrollVel.x;
#else
    m_scrollVel.x = 0;
#endif
  }
  float maxx = std::max(0.0f, child->GetSize().x - GetSize().x); 
  // depends on size of child and how much space there is to display it
  if (m_scrollPos.x > maxx)
  {
    m_scrollPos.x = maxx;
#ifdef BOUNCE
    m_scrollVel.x = -0.25f * m_scrollVel.x;
#else
    m_scrollVel.x = 0;
#endif
  }

  // Y axis
  if (m_scrollPos.y < 0)
  {
    m_scrollPos.y = 0;
#ifdef BOUNCE
    m_scrollVel.y = -0.25f * m_scrollVel.y;
#else
    m_scrollVel.y = 0;
#endif
  }
  
  float maxy = std::max(0.0f, child->GetSize().y - GetSize().y); // depends on size of child and how much space there is to display it
  if (m_scrollPos.y > maxy)
  {
    m_scrollPos.y = maxy;
#ifdef BOUNCE
    m_scrollVel.y = -0.25f * m_scrollVel.y;
#else
    m_scrollVel.y = 0;
#endif
  }
  SetLocalPos(m_scrollPos); // so combined pos for child is updated

  child->Draw();
}

void GuiScroll::Update()
{
}

void GuiScroll::OnScrollVelEvent(const Vec2f& scrollVel)
{
  m_scrollVel += scrollVel;

  const float MAX_SCROLL_VEL = 4.0f; // TODO

  // Enforce min/max
  m_scrollVel.x = std::max(-MAX_SCROLL_VEL, std::min(MAX_SCROLL_VEL, m_scrollVel.x));
  m_scrollVel.y = std::max(-MAX_SCROLL_VEL, std::min(MAX_SCROLL_VEL, m_scrollVel.y));

std::cout << "Scroll vel for " << GetName() << ": x:" << m_scrollVel.x 
  << " y: " << m_scrollVel.y << "\n"; 
}

bool GuiScroll::Load(File* f)
{
  std::string name;
  if (!f->GetDataLine(&name))
  {
    f->ReportError("Gui scroll: expected name");
    Assert(0);
    return false;
  }
  SetName(name);

  // One child (decorator)
  // No pos and size, so not using base class impl
  m_children.reserve(1);
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected child type for GUI Scroll");
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

  AddChild(e); 

  return true;
}

}

