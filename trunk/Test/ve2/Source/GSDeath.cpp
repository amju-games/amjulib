#include <AmjuFirst.h>
#include <Game.h>
#include "GSDeath.h"
#include "LocalPlayer.h"
#include "ObjectUpdater.h"
#include "GSWaitForNewLocation.h"
#include "LurkMsg.h"
#include <AmjuFinal.h>

namespace Amju
{
static const float MAX_TIME = 5.0f;

GSDeath::GSDeath()
{
}

void GSDeath::OnActive()
{
  GSGui::OnActive();

  m_time = 0;

  TheLurker::Instance()->Clear();
}

void GSDeath::OnDeactive()
{
  GSGui::OnDeactive();
}

void GSDeath::Update()
{
  // Just update timer. Fade out, then send msg to server that we can respawn.
  GSGui::Update();
  if (m_time > MAX_TIME)
  {
    // Reset player health
    // TODO

    // Go back to start
    // Send msg to reset player
    Player* p = GetLocalPlayer();
    int destLocation = 1;
    Vec3f destPos(0, 0, 0); // TODO Randomise
    SetLocalPlayerLocation(destLocation); // sets next state
    TheObjectUpdater::Instance()->SendChangeLocationReq(p->GetId(), destPos, destLocation);
    p->SetPos(destPos);
    p->MoveTo(destPos); 

    //TheGame::Instance()->SetCurrentState(TheGSWaitForNewLocation::Instance());
  }
}


}

