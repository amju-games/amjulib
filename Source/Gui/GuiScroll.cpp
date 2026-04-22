#include <AmjuFirst.h>
#include <algorithm>
#include <AmjuGL.h>
#include <LoadVec2.h>
#include <SoundManager.h>
#include <Timer.h>
#include "GuiScroll.h"
#include "GuiFactory.h"
#include <AmjuFinal.h>

// Bounce doesn't work yet
//#define BOUNCE
//#define SCROLL_DEBUG

namespace Amju
{
const char* GuiScroll::NAME = "gui-scroll";

std::string GuiScroll::s_tabStopSoundFilename;

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
  // Unfortunately, dx and dy are not reliable on all platforms
  Vec2f delta(Vec2f(ce.x, ce.y) - m_lastCursorPos);
  m_lastCursorPos = Vec2f(ce.x, ce.y);
  
  Assert(m_children.size() >= 1);
  GuiElement* child = m_children[0];

  if (m_leftDrag)
  {
    float scrollVelMultiplier = GetScrollVelMultiplier();
    if (CanScrollInX())
    {
      OnScrollVelEvent(Vec2f(delta.x * scrollVelMultiplier, 0));
    }
    if (CanScrollInY())
    {
      OnScrollVelEvent(Vec2f(0, delta.y * scrollVelMultiplier)); 
    }
  }

  return child->OnCursorEvent(ce); 
}

bool GuiScroll::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  m_lastCursorPos = Vec2f(mbe.x, mbe.y);
  
  Assert(m_children.size() >= 1);
  GuiElement* child = m_children[0];

  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT) 
  {
    if (mbe.isDown)
    {
      const Rect& r = GetScrollRect();
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

void GuiScroll::PlayTabStopSound(int tabNum) const
{
  // prevent repeats?
  //static int prevtabnum = -1;
  //if (tabNum == prevtabnum) return;
  //prevtabnum = tabNum;

  if (!s_tabStopSoundFilename.empty())
  {
    static SoundManager* s = TheSoundManager::Instance();
    s->PlayWav(s_tabStopSoundFilename);
  }
}

void GuiScroll::SetTabStop(int tabStop)
{
  m_lastTabStop = tabStop;
  float closestTabStop = m_tabStopSize.x * tabStop;
  m_scrollPos.x = closestTabStop;
}

void GuiScroll::UpdateForTabStops()
{
  const float stoppingDistance = GetStoppingDistance(); // 0.008f;
  const float stoppingVel = GetStoppingVel();  // 0.2f;
  const float speedBumpMult = GetSpeedBumpMult(); // 0.6f;

  // Calc the closest tab stop to the current pos
  const int closestTabNum = static_cast<int>(std::round(m_scrollPos.x / m_tabStopSize.x));
  const float closestTabStop = m_tabStopSize.x * closestTabNum;
  // Within stopping distance, with low velocity, we click into place.
  const float dist = std::abs(m_scrollPos.x - closestTabStop);
  
  // Don't do speedbump if being dragged
  if (   dist < stoppingDistance 
      && !m_leftDrag 
      && std::abs(m_scrollVel.x) > 0) 
  {
    // Speed bump: reduce vel
    m_scrollVel.x *= speedBumpMult;

    if (std::abs(m_scrollVel.x) < stoppingVel)
    {
      m_scrollPos.x = closestTabStop;
      m_scrollVel.x = 0;
      if (closestTabNum != m_lastTabStop)
      {
        // We have hit a tap stop and stopped: call the callback if it exists.
        if (m_tabStopCallback) m_tabStopCallback(this, closestTabNum);
        m_lastTabStop = closestTabNum;
        PlayTabStopSound(closestTabNum);
      }
    }
  }
}

void GuiScroll::Update()
{
  GuiComposite::Update();

  Assert(m_children.size() == 1);
  //GuiElement* child = m_children[0];

  const float dt = TheTimer::Instance()->GetDt();
  const float decel = GetDeceleration(); // 0.25f; // TODO
  const float minimumDeceleration = GetMinDeceleration(); // 0.7f;
  m_scrollVel *= std::max(minimumDeceleration, (1.0f - decel * dt));

  m_scrollPos += m_scrollVel * dt;

  // Check for tab stop in x..
  if (m_tabStopSize.x > 0)
  {
    UpdateForTabStops();
  }

  // x-axis
  // Bounce or stop on end reached
  if (m_scrollPos.x < -m_extents.x)
  {
    //if (m_tabStopCallback) m_tabStopCallback(this, 0);

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

  // Y axis -- TODO Tab stops!
  const float miny = std::min(0.f, m_extents.y);
  const float maxy = std::max(0.f, m_extents.y);

  if (m_scrollPos.y < miny)
  {
//std::cout << "Scroll: hit m_scrollPos.y = 0, stopping.\n";

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

  const float maxScrollVel = GetMaxScrollVel(); // 4.0f; // TODO

  // Enforce min/max
  m_scrollVel.x = std::max(-maxScrollVel, std::min(maxScrollVel, m_scrollVel.x));
  m_scrollVel.y = std::max(-maxScrollVel, std::min(maxScrollVel, m_scrollVel.y));

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
 // SetSize(extents); // ?
}

void GuiScroll::SetScrollVelMultiplier(float f)
{
  m_scrollVelMultiplier = f;
}

void GuiScroll::SetCanScrollInX(bool b)
{
  m_canScrollInX = b;
}

void GuiScroll::SetCanScrollInY(bool b)
{
  m_canScrollInY = b;
}

void GuiScroll::SetScrollRect(const Rect& r)
{
  m_scrollRect = r;
}

void GuiScroll::SetStoppingDistance(float f)
{
  m_stoppingDistance = f;
}

void GuiScroll::SetStoppingVel(float f)
{
  m_stoppingVel = f;
}

void GuiScroll::SetSpeedBumpMult(float f)
{
  m_speedBumpMult = f;
}

void GuiScroll::SetDeceleration(float f)
{
  m_deceleration = f;
}

void GuiScroll::SetMinDeceleration(float f)
{
  m_minDeceleration = f;
}

void GuiScroll::SetMaxScrollVel(float f)
{
  m_maxScrollVel = f;
}

float GuiScroll::GetScrollVelMultiplier() const
{
  return m_scrollVelMultiplier;
}

bool GuiScroll::CanScrollInX() const
{
  return m_canScrollInX;
}

bool GuiScroll::CanScrollInY() const
{
  return m_canScrollInY;
}

const Rect& GuiScroll::GetScrollRect() const
{
  return m_scrollRect;
}

float GuiScroll::GetStoppingDistance() const
{
  return m_stoppingDistance;
}

float GuiScroll::GetStoppingVel() const
{
  return m_stoppingVel;
}

float GuiScroll::GetSpeedBumpMult() const
{
  return m_speedBumpMult;
}

float GuiScroll::GetDeceleration() const
{
  return m_deceleration;
}

float GuiScroll::GetMinDeceleration() const
{
  return m_minDeceleration;
}

float GuiScroll::GetMaxScrollVel() const
{
  return m_maxScrollVel;
}
}
