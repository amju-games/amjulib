#include <AmjuFirst.h>
#include <algorithm>
#include <AmjuGL.h>
#include <LoadVec2.h>
#include <Timer.h>
#include "GuiScroll.h"
#include "GuiFactory.h"
#include <AmjuFinal.h>

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

bool GuiScroll::OnCursorEvent(const CursorEvent& ce)
{
  Assert(m_children.size() >= 1);
  GuiElement* child = m_children[0];

  if (m_leftDrag)
  {
    OnScrollVelEvent(Vec2f(ce.dx * 10.0f, ce.dy * 10.0f)); // TODO TEMP TEST x/y scroll flags
  }

  return child->OnCursorEvent(ce); 
}

bool GuiScroll::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  Assert(m_children.size() >= 1);
  GuiElement* child = m_children[0];

  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT) 
  {
    if (mbe.isDown)
    {
      Rect r = GetRect(this);
      m_leftDrag = r.IsPointIn(Vec2f(mbe.x, mbe.y));
    }
    else
    {
      m_leftDrag = false;
    }
  }

  return child->OnMouseButtonEvent(mbe); 
}

void GuiScroll::Draw()
{
  GuiComposite::Draw(); //child->Draw();
}

void GuiScroll::Update()
{
  GuiComposite::Update();

  Assert(m_children.size() == 1);
  //GuiElement* child = m_children[0];

  float dt = TheTimer::Instance()->GetDt();
  static const float DECEL = 0.5f; // TODO
  m_scrollVel *= std::max(0.0f, (1.0f - DECEL * dt));
  m_scrollPos += m_scrollVel * dt;

  // x-axis
  // Bounce or stop on end reached
  if (m_scrollPos.x < -m_extents.x)
  {
    m_scrollPos.x = -m_extents.x;
#ifdef BOUNCE
    m_scrollVel.x = -0.25f * m_scrollVel.x;
#else
    m_scrollVel.x = 0;
#endif
  }

  // depends on size of child and how much space there is to display it
  if (m_scrollPos.x > 0)
  {
    m_scrollPos.x = 0;
#ifdef BOUNCE
    m_scrollVel.x = -0.25f * m_scrollVel.x;
#else
    m_scrollVel.x = 0;
#endif
  }

  // Y axis
  float miny = std::min(0.f, m_extents.y);
  float maxy = std::max(0.f, m_extents.y);

  if (m_scrollPos.y < miny)
  {
std::cout << "Scroll: hit m_scrollPos.y = 0, stopping.\n";

    m_scrollPos.y = miny;
#ifdef BOUNCE
    m_scrollVel.y = -0.25f * m_scrollVel.y;
#else
    m_scrollVel.y = 0;
#endif
  }
  
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
}

void GuiScroll::StopScrolling()
{
  m_scrollVel = Vec2f(0, 0);
}

void GuiScroll::OnScrollVelEvent(const Vec2f& scrollVel)
{
  m_scrollVel += scrollVel;

  const float MAX_SCROLL_VEL = 4.0f; // TODO

  // Enforce min/max
  m_scrollVel.x = std::max(-MAX_SCROLL_VEL, std::min(MAX_SCROLL_VEL, m_scrollVel.x));
  m_scrollVel.y = std::max(-MAX_SCROLL_VEL, std::min(MAX_SCROLL_VEL, m_scrollVel.y));

#ifdef SCROLL_DEBUG
std::cout << "Scroll vel for " << GetName() << ": x:" << m_scrollVel.x 
  << " y: " << m_scrollVel.y << "\n"; 
#endif
}

bool GuiScroll::Load(File* f)
{
  // TODO do we want to name scroller? Probably yes, so we can reset it
  std::string name;
  if (!f->GetDataLine(&name))
  {
    f->ReportError("Gui scroll: expected name");
    Assert(0);
    return false;
  }
  SetName(name);

  // Initial position
  if (!LoadVec2(f, &m_scrollPos))
  {
    f->ReportError("Gui scroll: expected initial pos");
    Assert(0);
    return false;
  }

  // Extents
  if (!LoadVec2(f, &m_extents))
  {
    f->ReportError("Gui scroll: expected extents");
    Assert(0);
    return false;
  }

  return LoadOneChild(f);
}

void GuiScroll::InitScrollBar()
{
  // Not really a problem
//  Assert(!m_scrollBar); // already initialised

  // TODO This doesn't work!!!
  m_scrollBar = new GuiScrollBar;
  m_scrollBar->SetSize(Vec2f(0.1f, GetSize().y)); // for vertical
  m_scrollBar->SetLocalPos(Vec2f(0, 0)); // TODO
  m_scrollBar->SetParent(this); // needs to be on heap?
  m_scrollBar->Init();
  AddChild(m_scrollBar);
}

void GuiScroll::SetExtents(const Vec2f& extents)
{
  m_extents = extents;
 // SetSize(extents);
}

}


