#include "CollisionManager.h"
#include <iostream>

namespace Amju
{
typedef void (*CollisionHandler)(GameObject*, GameObject*);

bool CollisionManager::Add(
  const std::string& name1, const std::string& name2, CollisionHandler ch)
{
  TypeNamesPair tnp(name1, name2);
  m_map[tnp] = ch;
  return true;
}

bool CollisionManager::HandleCollision(GameObject* pGo1, GameObject* pGo2)
{
  TypeNamesPair tnp(pGo1->GetTypeName(), pGo2->GetTypeName());
  CollisionMap::iterator it = m_map.find(tnp);
  if (it != m_map.end())
  {
std::cout << "COLL_MGR: Calling collision handler for " << tnp.first << " and " << tnp.second << "\n";

    CollisionHandler ch = it->second;
    ch(pGo1, pGo2);
    return true;
  }
  // Try with types reversed
  std::swap(tnp.first, tnp.second);
  it = m_map.find(tnp);
  if (it != m_map.end())
  {
std::cout << "COLL_MGR: Calling collision handler for " << tnp.first << " and " << tnp.second << "\n";

    CollisionHandler ch = it->second;
    ch(pGo2, pGo1); // swapped
    return true;
  }
  
  return false;
}
}
