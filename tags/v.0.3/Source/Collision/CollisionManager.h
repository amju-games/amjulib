#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <map>
#include <string>
#include "Singleton.h"
#include "GameObject.h"

namespace Amju
{
class CollisionManager : public NonCopyable
{
private:
  CollisionManager() {}
  friend class Singleton<CollisionManager>;

public:
  typedef void (*CollisionHandler)(GameObject*, GameObject*);
  bool Add(const std::string& name1, const std::string& name2, CollisionHandler);
  bool HandleCollision(GameObject*, GameObject*);

private:
  typedef std::pair<std::string, std::string> TypeNamesPair;
  typedef std::map<TypeNamesPair, CollisionHandler> CollisionMap;
  CollisionMap m_map;
};

typedef Singleton<CollisionManager> TheCollisionManager;
}

#endif
