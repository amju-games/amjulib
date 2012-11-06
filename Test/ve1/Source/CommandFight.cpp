#include "CommandFight.h"
#include "FightReq.h"
#include "LocalPlayer.h"

namespace Amju
{
static const float YSIZE = 200.0f; // TODO TEMP TEST

bool CommandFight::Do()
{
  // Show visual effect. Send request to server.
  Player* loc = GetLocalPlayer();
  Vec3f pos = m_opponent->GetPos();
  pos.y += 40.0f; // TODO TEMP TEST
  loc->MoveTo(pos);

  Vec3f vel = loc->GetVel();
  vel.y += YSIZE;
  loc->SetVel(vel);

  SendFightReq(m_opponent);

//    m_opponent->ShowAttacked();
   
  return false;
}
}


