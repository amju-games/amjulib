#include <CollisionManager.h>
#include "Player.h"
#include "Building.h"

namespace Amju
{
void CollidePlayerBuilding(Player* player, Building* b)
{

  player->SetIsColliding(b);

}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, Building::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerBuilding);
} 
