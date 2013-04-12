#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <map>
#include <set>
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

  // TODO Flag so we can handle collision only on first contact, or every frame as appropriate.
  bool Add(const std::string& name1, const std::string& name2, CollisionHandler);

  // Called from collision detection code
  bool HandleCollision(GameObject*, GameObject*);

  // Call each frame before testing for collisions 
  void InitFrame(); 

private:
  typedef std::pair<std::string, std::string> TypeNamesPair;
  typedef std::map<TypeNamesPair, CollisionHandler> CollisionMap;
  CollisionMap m_map;

  // remember pairs of colliding objects so we only notify on first contact
  typedef std::pair<int, int> IdPair;
  typedef std::set<IdPair> IdPairSet; 
  IdPairSet m_collisionsThisFrame;
  IdPairSet m_collisionsLastFrame;
};

typedef Singleton<CollisionManager> TheCollisionManager;
}

#endif
