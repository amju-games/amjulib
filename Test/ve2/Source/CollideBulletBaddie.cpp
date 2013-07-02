#include <AmjuFirst.h>
#include <Game.h>
#include <CollisionManager.h>
#include "Bullet.h"
#include "Baddie.h"
#include <AmjuFinal.h>

namespace Amju
{
void CollideBulletBaddie(Bullet* bullet, Baddie* baddie)
{
  if (bullet->IsHidden() || baddie->IsHidden())
  {
    return;
  }

  baddie->OnCollideBullet();

  // Get rid of bullet
  bullet->OnLocationExit();
  TheGame::Instance()->EraseGameObject(bullet->GetId());
}

static bool b1 = TheCollisionManager::Instance()->Add(
  Bullet::TYPENAME, Baddie::TYPENAME, (CollisionManager::CollisionHandler)CollideBulletBaddie);
} 
