#include <algorithm>
#include <AmjuGL.h>
#include <Timer.h>
#include "GuiScroll.h"
#include "GuiFactory.h"

namespace Amju
{
const char* GuiScroll::NAME = "gui-scroll";

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
  return false;
}

bool GuiScroll::OnCursorEvent(const CursorEvent& ce)
{
  Assert(m_children.size() == 1);
  GuiElement* child = m_children[0];
  
  CursorEvent e(ce);
//  e.x += m_scrollPos.x;
//  e.y -= m_scrollPos.y;

//std::cout << "Mouse: orig x: " << ce.x << " y: " << ce.y << " new x: " << e.x << " y: " << e.y << "\n";

  return child->OnCursorEvent(e); 
}

bool GuiScroll::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  Assert(m_children.size() == 1);
  GuiElement* child = m_children[0];

  MouseButtonEvent e(mbe);
//  e.x += m_scrollPos.x;
//  e.y -= m_scrollPos.y;
std::cout << "BUTTON EVENT: m_scrollPos.y: " << m_scrollPos.y;
std::cout << " Mouse: orig x: " << mbe.x << " y: " << mbe.y << " new x: " << e.x << " y: " << e.y << "\n";

  return child->OnMouseButtonEvent(e); 
}

void GuiScroll::Draw()
{
  // Update not called. TODO
  float dt = TheTimer::Instance()->GetDt();
  static const float DECEL = 0.5f; // TODO
  m_scrollVel *= std::max(0.0f, (1.0f - DECEL * dt));
  m_scrollPos += m_scrollVel * dt;

  // Enforce boundary - TODO other edges
//  m_scrollPos.x = std::max(m_scrollPos.x, 0.0f);
//  m_scrollPos.y = std::max(m_scrollPos.y, 0.0f);

///  m_scrollPos.y = 0.2f; // TODO TEMP TEST

  // TODO
  /*
  // Bounce on end reached ??
  if (m_scrollPos.x < 0)
  {
    m_scrollVel.x = -0.5f * m_scrollVel.x;
  }
  ...
  */

  SetLocalPos(m_scrollPos); // so combined pos for child is updated

  Assert(m_children.size() == 1);
  GuiElement* child = m_children[0];

  child->Draw();

/*
  // translate - must compensate for mouse coords etc
  AmjuGL::PushMatrix();
  AmjuGL::Translate(m_scrollPos.x, m_scrollPos.y, 0);
  child->Draw();
  AmjuGL::PopMatrix();
*/
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


