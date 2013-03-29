#include <AmjuFirst.h>
#include <CollisionManager.h>
#include "Player.h"
#include "Building.h"
#include <AmjuFinal.h>

namespace Amju
{
void CollidePlayerBuilding(Player* player, Building* b)
{
  if (player->IsHidden() || b->IsHidden())
  {
    return;
  }

  player->SetIsColliding(b);
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, Building::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerBuilding);
} 
