#include <AmjuFirst.h>
#include <CollisionManager.h>
#include "Player.h"
#include "Food.h"
#include <AmjuFinal.h>

namespace Amju
{
void CollidePlayerFood(Player* player, Food* f)
{
  if (player->IsHidden() || f->IsHidden())
  {
    return;
  }

  player->OnCollideFood(f);
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, Food::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerFood, AMJU_FIRST_CONTACT_ONLY);
} 
