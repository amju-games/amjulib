#include <LoadVec2.h>
#include <LoadRect.h>
#include "BaddieBehaviourPatrol.h"

namespace Amju
{
BaddieBehaviour* CreateBBPatrol()
{
  return new BBPatrol;
}

const char* BBPatrol::TYPENAME = "patrol";

static bool registered = TheBBFactory::Instance()->Add(BBPatrol::TYPENAME, CreateBBPatrol);

BBPatrol::BBPatrol()
{
  m_reverse = false;
}

void BBPatrol::Update()
{
  m_baddie->SetVel(Vec3f(m_patrolDir.x, 0, m_patrolDir.y));

  Vec3f pos = m_baddie->GetPos();
  if (pos.x < m_rect.GetMin(0))
  {
    pos.x = m_rect.GetMin(0);
    m_patrolDir.x = -m_patrolDir.x;
    m_baddie->SetPos(pos);
  }
  else if (pos.x > m_rect.GetMax(0))
  {
    pos.x = m_rect.GetMax(0);
    m_patrolDir.x = -m_patrolDir.x;
    m_baddie->SetPos(pos);
  }

  if (pos.y < m_rect.GetMin(1))
  {
    pos.y = m_rect.GetMin(1);
    m_patrolDir.y = -m_patrolDir.y;
    m_baddie->SetPos(pos);
  }
  else if (pos.y > m_rect.GetMax(1))
  {
    pos.y = m_rect.GetMax(1);
    m_patrolDir.y = -m_patrolDir.y;
    m_baddie->SetPos(pos);
  }
}

bool BBPatrol::Load(File* f)
{
  // Direction vec
  if (!LoadVec2(f, &m_patrolDir))
  {
    f->ReportError("Failed to load baddie patrol dir");
    return false;
  }

  // Bounding rect
  if (!LoadRect(f, &m_rect))
  {
    f->ReportError("Failed to load baddie patrol rect");
    return false;
  }

  // Reverse flag
  int i = 0;
  if (!f->GetInteger(&i))
  {
    f->ReportError("Expected baddie patrol reverse flag");
    return false;
  }
  m_reverse = (i != 0);

  return true;
}

}
