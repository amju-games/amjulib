#include "BaddieBehaviourChase.h"
#include "LocalPlayer.h"

namespace Amju
{
BaddieBehaviour* CreateBBChase()
{
  return new BBChase;
}

const char* BBChase::TYPENAME = "chase";

static bool registered = TheBBFactory::Instance()->Add(BBChase::TYPENAME, CreateBBChase);

BBChase::BBChase()
{
  m_chaseSpeed = 0;
}

void BBChase::Update()
{
  // Chase behaviour
  Player* lp = GetLocalPlayer();
  if (lp)
  {
    Vec3f v = lp->GetPos() - m_baddie->GetPos();
    v.Normalise();
    v *= m_chaseSpeed;
    m_baddie->SetVel(v);
  }
  else
  {
    m_baddie->SetVel(Vec3f()); // Or move more slowly??
  }
}

bool BBChase::Load(File* f)
{
  if (!f->GetFloat(&m_chaseSpeed))
  {
    f->ReportError("Expected chase speed");
    return false;
  }

  return true;
}
}
