#include <AmjuFirst.h>
#include <CollisionManager.h>
#include "Player.h"
#include "Treasure.h"
#include <AmjuFinal.h>

namespace Amju
{
void CollidePlayerTreasure(Player* player, Treasure* t)
{
  if (player->IsHidden() || t->IsHidden())
  {
    return;
  }

  t->OnCollidePlayer(player);
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, Treasure::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerTreasure);
} 
