#include <CollisionManager.h>
#include "Player.h"
#include "Furniture.h"
#include "CollideObjectFurniture.h"

namespace Amju
{
void CollidePlayerFurniture(Player* player, Furniture* f)
{
  if (player->IsHidden() || f->IsHidden())
  {
    return;
  }

  // Move back away from furniture, and then adjust heading if moving.

//  f->OnPlayerCollision(player);
  player->SetIsColliding(f);

//  CollideObjectFurniture(player, f);
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, Furniture::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerFurniture);
} 
