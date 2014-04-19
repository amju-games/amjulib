#include <Timer.h>
#include <Sign.h>
#include <LoadVec2.h>
#include "GuiElastic.h"

namespace Amju
{
const char* GuiElastic::NAME = "elastic";

GuiElastic::GuiElastic() : m_bounceX(0), m_bounceY(0)
{
}

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

  GuiElement* child = GetChild(0);

  // Name: create one for convenience in gui file
  std::string name = "elastic_for_" + child->GetName();
  SetName(name);

  const float VEL_MULT = 5.0f; // TODO
  m_vel = -m_localpos * VEL_MULT;
  m_origVel = m_vel;

#ifdef ELASTIC_DEBUG
std::cout << "Elastic: " << m_name << " vel: " << m_vel.x << ", " << m_vel.y << "\n";
#endif

  return true;
}

void GuiElastic::Draw()
{
  float dt = TheTimer::Instance()->GetDt();
  m_vel += m_acc * dt;
  m_localpos += m_vel * dt;
  const float DAMP = 0.25f; // TODO
  const float ACC_MULT = 2.0f;  // TODO
  const int MAX_BOUNCE = 5;

  if (m_bounceX < MAX_BOUNCE)
  { 
    if ((m_localpos.x < 0 && m_origVel.x < 0) ||
        (m_localpos.x > 0 && m_origVel.x > 0))
    {
#ifdef ELASTIC_DEBUG
std::cout << "Elastic: " << m_name << " bounce in X..\n";
#endif

      m_acc.x = m_vel.x * ACC_MULT;
      m_vel.x = -m_vel.x * DAMP;
      m_localpos.x = 0;
      m_bounceX++;
    
      if (m_bounceX == MAX_BOUNCE)
      {
        m_acc.x = 0;
        m_vel.x = 0;
        m_localpos.x = 0;
      }
    }
  }

  if (m_bounceY < MAX_BOUNCE)
  { 
    if ((m_localpos.y < 0 && m_origVel.y < 0) ||
        (m_localpos.y > 0 && m_origVel.y > 0))
    {
#ifdef ELASTIC_DEBUG
std::cout << "Elastic: " << m_name << " bounce in Y..\n";
#endif

      m_acc.y = m_vel.y * ACC_MULT;
      m_vel.y = -m_vel.y * DAMP;
      m_localpos.y = 0;
      m_bounceY++;
    
      if (m_bounceY == MAX_BOUNCE)
      {
        m_acc.y = 0;
        m_vel.y = 0;
        m_localpos.y = 0;
      }
    }
  }

  GuiComposite::Draw();
}
}

