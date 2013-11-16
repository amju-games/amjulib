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
// Type of function which returns true if the 2 game objects collided this frame.
// Default for this function in CollisionDetector is static AABB intersection test.
typedef bool (*CollideFunc)(GameObject*, GameObject*);

// CollisionDetector:
// Base class for types of collision detection algos, e.g. Sweep and Prune vs
//  brute force. Create type you want (with new) and set in CollisionManager.
class CollisionDetector : public RefCounted
{
public:
  CollisionDetector();
  virtual ~CollisionDetector() = 0;
  virtual void Clear() {} // call at end of level
  virtual void Update() = 0; // call each frame to check for collisions

  // Call to set a different collision test, e.g. dynamic.
  void SetCollideFunc(CollideFunc cf);

protected:
  CollideFunc m_collideFunc;
};

// CollisionManager:
// Tells the chosen collision detector to update.
// Handles collisions between 2 objects, calling handler function if one
//  has been registered. (Double dispatch.)
// When you add a handler function, specify whether to only call handler function 
//  on first collision, or every time
static const bool AMJU_FIRST_CONTACT_ONLY = true;
static const bool AMJU_EVERY_CONTACT = false;

class CollisionManager : public NonCopyable
{
private:
  CollisionManager() {}
  friend class Singleton<CollisionManager>;

public:
  // Type of function that responds to a collision between 2 GameObjects
  typedef void (*CollisionHandler)(GameObject*, GameObject*);

  // Register a collision handler function for 2 types of game object.
  // All GameObject types have a string ID, which you get from GetTypeName().
  // Flag so we can handle collision only on first contact, 
  //  or every contacting frame as appropriate.
  bool Add(const std::string& name1, const std::string& name2, CollisionHandler, bool handleFirstContactOnly = AMJU_FIRST_CONTACT_ONLY);

  // Called from collision detector, so has to be public
  bool HandleCollision(GameObject*, GameObject*);

  // Call every frame
  void Update();

  // Set Collision Detection algo, e.g. Brute Force, Sweep and Prune
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

  // Flags for pairs of types which should only be notified on first contact
  typedef std::set<TypeNamesPair> FirstContactOnlyFlags;
  FirstContactOnlyFlags m_firstContactOnlyFlags;

  RCPtr<CollisionDetector> m_cd;
};

typedef Singleton<CollisionManager> TheCollisionManager;
}

#endif

