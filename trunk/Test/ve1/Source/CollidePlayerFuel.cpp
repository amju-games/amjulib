#include <CollisionManager.h>
#include <SoundManager.h>
#include "Player.h"
#include "FuelCell.h"
#include "FirstTimeMsg.h"

namespace Amju
{
void CollidePlayerFuel(Player* player, FuelCell* f)
{
  if (player->IsHidden() || f->IsHidden())
  {
    return;
  }

  f->SetHidden(true);
  TheSoundManager::Instance()->PlayWav("button112.wav"); // TODO

  FirstTimeMsgThisSession(
    "Great, this is a fuel cell! We need these. Find as many as you can!", 
    MsgNum(10),
    false);

  player->OnCollideFuel(f);
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, FuelCell::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerFuel);
} 
