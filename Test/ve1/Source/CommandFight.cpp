#include "CommandFight.h"
#include "FightReq.h"
#include "LocalPlayer.h"
#include "ObjectUpdater.h"

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

  int location = GetLocalPlayerLocation(); 
  TheObjectUpdater::Instance()->SendPosUpdateReq(loc->GetId(), pos, location);

  // Causes player to float way up into the air
//  Vec3f vel = loc->GetVel();
//  vel.y += YSIZE;
//  loc->SetVel(vel);

  SendFightReq(m_opponent);

  // Do this when we get server response
  //m_opponent->ShowAttacked();
   
  return false;
}
}


