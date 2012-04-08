#include "BruteForce.h"
#include "CollisionManager.h"

namespace Amju
{
void BruteForce(GameObjects* gameobjs)
{
  // Copy so iterator not invalidated
  GameObjects gos(*gameobjs);

  for (GameObjects::iterator it = gos.begin(); it != gos.end(); ++it)
  {
    PGameObject go1 = it->second;

    GameObjects::iterator jt = it;
    ++jt;
    for ( ; jt != gos.end(); ++jt)
    {
      PGameObject go2 = jt->second;

      AABB* aabb1 = go1->GetAABB();
      AABB* aabb2 = go2->GetAABB();

      if (aabb1 && aabb2 && aabb1->Intersects(*aabb2))
      {
        TheCollisionManager::Instance()->HandleCollision(go1, go2);
      }
    }
  }
}
}

