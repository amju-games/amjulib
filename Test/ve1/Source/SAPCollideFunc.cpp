#include "SAPCollideFunc.h"
#include <GameObject.h>
#include <AABB.h>

namespace Amju
{
bool SAPCollideFunc(GameObject* go1, GameObject* go2)
{
  Assert(go1);
  Assert(go2);

  AABB* aabb1 = go1->GetAABB();
  if (!aabb1)
  {
    return false;
  }
  AABB* aabb2 = go2->GetAABB();
  if (!aabb2)
  {
    return false;
  }
  bool b = aabb1->Intersects(*aabb2);
#ifdef _DEBUG
std::cout << "COLLISION! " << go1->GetId() << " and " << go2->GetId() << "\n";
#endif

  return b;
}

}

