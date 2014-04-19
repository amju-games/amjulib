#include <AmjuFirst.h>
#include <Game.h>
#include "Ve1BruteForce.h"
#include "CollisionManager.h"
#include "Ve1Object.h"
#include <AmjuFinal.h>

namespace Amju
{
void Ve1BruteForce::Update()
{
  static Game* game = TheGame::Instance();
  GameObjects* gameobjs = game->GetGameObjects();

  // Copy so iterator not invalidated
  GameObjects gos(*gameobjs);

  for (GameObjects::iterator it = gos.begin(); it != gos.end(); ++it)
  {
    Assert(dynamic_cast<Ve1Object*>(it->second.GetPtr()));
    Ve1Object* go1 = (Ve1Object*)it->second.GetPtr();
    Assert(go1);

    if (go1->IsHidden())
    {
      continue;
    }

    GameObjects::iterator jt = it;
    ++jt;
    for ( ; jt != gos.end(); ++jt)
    {
      Assert(dynamic_cast<Ve1Object*>(jt->second.GetPtr()));
      Ve1Object* go2 = (Ve1Object*)jt->second.GetPtr();
      Assert(go2);

      if (go2->IsHidden())
      {
        continue;
      }

      const AABB& aabb1 = go1->GetAABB();
      const AABB& aabb2 = go2->GetAABB();

      if (aabb1.Intersects(aabb2))
      {
        TheCollisionManager::Instance()->HandleCollision(go1, go2);
      }
    }
  }
}
}

