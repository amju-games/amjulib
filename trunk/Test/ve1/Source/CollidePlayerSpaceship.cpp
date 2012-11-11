#include <CollisionManager.h>
#include "Player.h"
#include "Spaceship.h"
#include "FirstTimeMsg.h"

namespace Amju
{
void CollidePlayerSpaceship(Player* player, Spaceship* spaceship)
{
  if (player->IsHidden() || spaceship->IsHidden())
  {
    return;
  }

  player->SetIsColliding(spaceship);

  FirstTimeMsgThisSession("This is your spaceship! It needs fuel. Please find fuel cells and bring them back here!", MsgNum(3), false);

  // TODO carrying fuel cell ?
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Player::TYPENAME, Spaceship::TYPENAME, (CollisionManager::CollisionHandler)CollidePlayerSpaceship);

}

