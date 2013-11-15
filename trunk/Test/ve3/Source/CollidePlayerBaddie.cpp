#include <AmjuFirst.h>
#include "CollisionManager.h"
#include "Player.h"
#include "Baddie.h"
#include <AmjuFinal.h>

namespace Amju
{
void CollidePlayerBaddie(Player* player, Baddie* b)
{
  if (player->IsHidden() || b->IsHidden())
  {
    return;
  }

  if (player->IsLocalPlayer())
  {
    player->OnCollideBaddie(b);
  }
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, Baddie::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerBaddie);
} 
