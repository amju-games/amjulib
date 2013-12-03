#include <Timer.h>
#include <Sign.h>
#include <LoadVec2.h>
#include "GuiElastic.h"

namespace Amju
{
const char* GuiElastic::NAME = "elastic";

bool GuiElastic::Load(File* f)
{
  // Load pos offset, which is reduced to (0, 0);
  if (!LoadVec2(f, &m_localpos))
  {
    f->ReportError("Expected elastic offset");
    return false;
  }

  if (!LoadOneChild(f))
  {
    return false;
  }

  // Name: create one for convenience in gui file
  GuiElement* child = GetChild(0);
  std::string name = "elastic_for_" + child->GetName();

  m_vel = child->GetLocalPos() - m_localpos; 
  return true;
}

void GuiElastic::Draw()
{
  float dt = TheTimer::Instance()->GetDt();
  Vec2f oldPos = m_localpos;
  m_vel += m_acc * dt;
  m_localpos += m_vel * dt;
  const float DAMP = 0.9f; // TODO
  if (Sign(m_localpos.x) != Sign(oldPos.x))
  {
    m_acc.x = m_vel.x;
    m_vel.x = -m_vel.x * DAMP;
    m_localpos.x = 0;
  }
  if (Sign(m_localpos.y) != Sign(oldPos.y))
  {
    m_acc.y = m_vel.y;
    m_vel.y = -m_vel.y * DAMP;
    m_localpos.y = 0;
  }
  GuiComposite::Draw();
}
}

