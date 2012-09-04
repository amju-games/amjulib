#include "LocalPlayer.h"
#include "ObjectManager.h"

namespace Amju
{
static int localPlayerId = -1;
static int localPlayerLocation = -1;

const char* TOTAL_PLAY_TIME_KEY = "total_play_time";

int GetLocalPlayerLocation()
{
  //Assert(localPlayerLocation > 0);
  return localPlayerLocation; 
}

void SetLocalPlayerLocation(int lpl)
{
  localPlayerLocation = lpl;
  TheObjectManager::Instance()->SetLocalPlayerLocation(lpl);
}

bool Player::IsLocalPlayer() const
{
  return (m_id == localPlayerId);
}
    
Player* GetLocalPlayer()
{
  if (localPlayerId == -1)
  {
    return 0;
  }

  GameObject* p = TheObjectManager::Instance()->GetGameObject(localPlayerId);

  // May return 0
  return (Player*)p;
}

void SetLocalPlayerId(int id)
{
  localPlayerId = id;
}

int GetLocalPlayerId()
{
  return localPlayerId;
}

}


