#include "BruteForce.h"
#include "CollisionManager.h"
#include "Ve1Object.h"

namespace Amju
{
void BruteForce(GameObjects* gameobjs)
{
  // Copy so iterator not invalidated
  static GameObjects gos; // static to reduce reallocs
  gos = *gameobjs;

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

