#include "LocalPlayer.h"
#include "ObjectManager.h"
#include "ObjectUpdater.h"
#include "ROConfig.h"
#include "Useful.h"
#include "GSStartGame.h"

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

void ResetLocalPlayer()
{
  int startLoc = ROConfig()->GetInt("start-loc");
  TheGSStartGame::Instance()->SetStartLoc(startLoc); 

  std::string pos = ROConfig()->GetValue("start-pos");
  Vec3f startPos(ToVec3(pos));

  Player* player = GetLocalPlayer();
  TheObjectUpdater::Instance()->SendPosUpdateReq(GetLocalPlayerId(), startPos, startLoc);
  if (player)
  {
    player->SetPos(startPos); // client side predict - respond immediately
  }
}

}


