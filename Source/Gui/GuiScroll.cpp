#include <AmjuGL.h>
#include <Timer.h>
#include "GuiScroll.h"

namespace Amju
{
const char* GuiScroll::NAME = "gui-scroll";

void GuiScroll::Draw()
{
  Assert(m_children.size() == 1);

  AmjuGL::PushMatrix();
  AmjuGL::Translate(m_scrollPos.x, m_scrollPos.y, 0);
  m_children[0]->Draw();
  AmjuGL::PopMatrix();
}

void GuiScroll::Update()
{
  float dt = TheTimer::Instance()->GetDt();
  static const float DECEL = 0.2f; // TODO
  m_scrollVel *= DECEL * dt;
  m_scrollPos += m_scrollVel * dt;
}


void GuiScroll::OnScrollVelEvent(const Vec2f& scrollVel)
{
  m_scrollVel += scrollVel;
}

}


