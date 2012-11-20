#include <CollisionManager.h>
#include <SoundManager.h>
#include "Player.h"
#include "FuelCell.h"

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

  player->OnCollideFuel(f);
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, FuelCell::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerFuel);
} 
