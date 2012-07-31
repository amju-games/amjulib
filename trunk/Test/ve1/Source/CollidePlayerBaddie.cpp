#include "CollisionManager.h"
#include "Player.h"
#include "Baddie.h"
#include "CollideObjectFurniture.h"

namespace Amju
{
void CollidePlayerBaddie(Player* player, Baddie* b)
{
  if (player->IsHidden() || b->IsHidden())
  {
    return;
  }

  player->SetIsColliding(b);
  CollideObjectFurniture(player, b); // player moves back
  // TODO This is a baddie, so we should lose health etc
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, Baddie::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerBaddie);
} 
