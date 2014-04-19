#include <AmjuFirst.h>
#include "CollisionManager.h"
#include "Player.h"
#include "Collect.h"
#include <AmjuFinal.h>

namespace Amju
{
void CollidePlayerCollect(Player* player, Collect* c)
{
  if (player->IsHidden() || c->IsHidden())
  {
    return;
  }

  c->OnPlayerCollision(player);
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, Collect::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerCollect, AMJU_FIRST_CONTACT_ONLY);
} 
