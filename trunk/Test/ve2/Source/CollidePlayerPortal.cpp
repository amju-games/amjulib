#include <AmjuFirst.h>
#include "CollisionManager.h"
#include "Player.h"
#include "Portal.h"
#include <AmjuFinal.h>

namespace Amju
{
void CollidePlayerPortal(Player* player, Portal* portal)
{
  if (player->IsHidden())
  {
    return;
  }

  portal->OnPlayerCollision(player);
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, Portal::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerPortal);
} 
