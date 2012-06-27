#include <CollisionManager.h>
#include "Player.h"
#include "Furniture.h"
#include "CollideObjectFurniture.h"

namespace Amju
{
void CollidePlayerFurniture(Player* player, Furniture* f)
{
  // Move back away from furniture, and then adjust heading if moving.

  f->OnPlayerCollision(player);

  CollideObjectFurniture(player, f);

  // This doesn't work well. Better if the player adjusts heading once he is NOT colliding
  //  with anything ? This could cause other probs, because you could end up never adjusting.
  // How about trying to adjust, but if you are not getting closer to the target pos, stop moving.

  // Adjust heading
//  if (player->IsMoving())
//  {
//    player->MoveTo(player->GetTargetPos());
//  }
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, Furniture::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerFurniture);
} 
