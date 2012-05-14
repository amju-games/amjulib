#include <iostream>
#include "PickObject.h"
#include <GameObject.h>
#include <Game.h>
#include <Unproject.h>
#include <Line3.h>
#include <ClipLineSegBox.h>
#include "Ve1Object.h"

#define PICK_DEBUG

namespace Amju
{
GameObject* PickObject(const Vec2f& mouseScreen)
{
  Vec3f mouseWorldNear;
  Vec3f mouseWorldFar;

  Unproject(mouseScreen, 0, &mouseWorldNear);
  Unproject(mouseScreen, 1, &mouseWorldFar);
  LineSeg lineSeg(mouseWorldNear, mouseWorldFar);

  GameObject* selectedObj = 0;
  GameObjects* objs = TheGame::Instance()->GetGameObjects();
  float bestDist = 999999.9f;
  for (GameObjects::iterator it = objs->begin(); it != objs->end(); ++it)
  {
    GameObject* pgo = it->second;
    Assert(pgo);
    Ve1Object* v = dynamic_cast<Ve1Object*>(pgo);
    Assert(v);
    if (!v->IsPickable())
    {
#ifdef PICK_DEBUG
std::cout << " Obj " << pgo->GetId() << " is not pickable.\n";
#endif
      continue;
    }

    AABB* aabb = pgo->GetAABB();
    if (aabb && Clip(lineSeg, *aabb, 0))
    {
#ifdef PICK_DEBUG
std::cout << " Obj " << pgo->GetId() << " IS PICKED!\n";
#endif

      // Line seg intersects this box
      // Choose object whose centre (position) is closest to line seg..?
      float dist = LineSeg(mouseWorldNear, mouseWorldFar).SqDist(pgo->GetPos());
      //float dist = (mouseWorldNear - pgo->GetPos()).SqLen(); // pick closest
      if (dist < bestDist)
      {
#ifdef PICK_DEBUG
std::cout << " Obj " << pgo->GetId() << " AND IS CLOSEST!\n";
#endif

        bestDist = dist;
        selectedObj = pgo;
      }
    }
    else if (aabb)
    {
std::cout << " Obj " << pgo->GetId() << " is not picked.\n";
    }
    else
    {
std::cout << " Obj " << pgo->GetId() << " has no AABB.\n";
    }
  }

  return selectedObj;
}
}



