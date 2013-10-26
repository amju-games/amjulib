#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <map>
#include <set>
#include <string>
#include <RCPtr.h>
#include <Singleton.h>
#include <GameObject.h>

namespace Amju
{
// CollisionDetector:
// Base class for types of collision detection algos, e.g. Sweep and Prune vs
//  brute force. Create type you want and set in CollisionManager.

// Type of function which returns true if the 2 game objects collided this frame.
// Default for this function in CollisionDetector is static AABB intersection test.
typedef bool (*CollideFunc)(GameObject*, GameObject*);

class CollisionDetector : public RefCounted
{
public:
  CollisionDetector();
  virtual ~CollisionDetector() = 0;
  virtual void Clear() {} // call at end of level
  virtual void Update() = 0; // call each frame to check for collisions

  void SetCollideFunc(CollideFunc cf);

protected:
  CollideFunc m_collideFunc;
};

// CollisionManager:
// Tells the chosen collision detector to update.
// Handles collisions between 2 objects, calling handler function if one
//  has been registered. Double dispatch.
// Currently only calls handler function on first collision, not every time
//  -- this should be a flag so client code can choose.
class CollisionManager : public NonCopyable
{
private:
  CollisionManager() {}
  friend class Singleton<CollisionManager>;

public:
  // Type of function that responds to a collision between 2 GameObjects
  typedef void (*CollisionHandler)(GameObject*, GameObject*);

  // Register a collision handler function for 2 types of game object.
  // All GameObjects have a string, which you get from GetTypeName()
  // TODO Flag so we can handle collision only on first contact, 
  //  or every frame as appropriate.
  bool Add(const std::string& name1, const std::string& name2, CollisionHandler);

  // Called from collision detector, so has to be public
  bool HandleCollision(GameObject*, GameObject*);

  // Call every frame
  void Update();

  void SetCollisionDetector(CollisionDetector* cd);
  CollisionDetector* GetCollisionDetector();

private:

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

  RCPtr<CollisionDetector> m_cd;
};

typedef Singleton<CollisionManager> TheCollisionManager;
}

#endif
