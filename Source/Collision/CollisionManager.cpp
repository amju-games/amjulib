#include <AmjuFirst.h>
#include "CollisionManager.h"
#include <iostream>
#include <AmjuFinal.h>

#define COLL_MGR_DEBUG
//#define COLL_MGR_DEBUG_FIRST_CONTACT

namespace Amju
{
static bool StaticAABBIntersectionTest(GameObject* go1, GameObject* go2)
{
  Assert(go1);
  Assert(go2);
  const AABB& aabb1 = go1->GetAABB();
  const AABB& aabb2 = go2->GetAABB();
  return (aabb1.Intersects(aabb2));
}

CollisionDetector::CollisionDetector()
{
  m_collideFunc = StaticAABBIntersectionTest;
}

CollisionDetector::~CollisionDetector()
{
}

void CollisionDetector::SetCollideFunc(CollideFunc cf)
{
  m_collideFunc = cf;
}

void CollisionManager::Update()
{
  if (m_cd)
  {
    InitFrame();
    m_cd->Update();
  }
}

void CollisionManager::SetCollisionDetector(CollisionDetector* cd)
{
  m_cd = cd;
}

CollisionDetector* CollisionManager::GetCollisionDetector()
{
  return m_cd;
}

bool CollisionManager::Add(
  const std::string& name1, const std::string& name2, CollisionHandler ch,bool handleFirstContactOnly)
{
  if (handleFirstContactOnly)
  {
    // Store both ways round
    m_firstContactOnlyFlags.insert(TypeNamesPair(name1, name2));
    m_firstContactOnlyFlags.insert(TypeNamesPair(name2, name1));
  }

  TypeNamesPair tnp(name1, name2);
  m_map[tnp] = ch;
  return true;
}

void CollisionManager::InitFrame()
{
  // TODO check for last contact: any pairs in m_collisionsLastFrame but 
  //  NOT in m_collisionsThisFrame
  //  have just left contact with each other, so we could call a handler for that.

  m_collisionsLastFrame = m_collisionsThisFrame;
  m_collisionsThisFrame.clear();
}

bool CollisionManager::HandleCollision(GameObject* pGo1, GameObject* pGo2)
{
  TypeNamesPair tnp(pGo1->GetTypeName(), pGo2->GetTypeName());
  // Check for first contact if this has been set for this pair of types
  if (m_firstContactOnlyFlags.count(tnp) > 0)
  {
    IdPair idpair(pGo1->GetId(), pGo2->GetId());

    m_collisionsThisFrame.insert(idpair);
    if (m_collisionsLastFrame.count(idpair))
    {
#ifdef COLL_MGR_DEBUG_FIRST_CONTACT
      std::cout << "COLL_MGR: Not handling collision between objs " << pGo1->GetId() << " and " << pGo2->GetId() << " as not first contact.\n";
#endif
      return false;  
    }
    std::swap(idpair.first, idpair.second);
    if (m_collisionsLastFrame.count(idpair))
    {
#ifdef COLL_MGR_DEBUG_FIRST_CONTACT
      std::cout << "COLL_MGR: Not handling collision between objs " << pGo1->GetId() << " and " << pGo2->GetId() << " as not first contact.\n";
#endif
      return false;  
    }
  }

  CollisionMap::iterator it = m_map.find(tnp);
  if (it != m_map.end())
  {
#ifdef COLL_MGR_DEBUG
std::cout << "COLL_MGR: Calling collision handler for obj " << pGo1->GetId() << " (" << tnp.first << ") and " << pGo2->GetId() << " (" << tnp.second << ")\n";
#endif

    CollisionHandler ch = it->second;
    ch(pGo1, pGo2);
    return true;
  }
  // Try with types reversed
  std::swap(tnp.first, tnp.second);

  it = m_map.find(tnp);
  if (it != m_map.end())
  {
#ifdef COLL_MGR_DEBUG
std::cout << "COLL_MGR: Calling collision handler for obj " << pGo2->GetId() << " (" << tnp.first << ") and " << pGo1->GetId() << " (" << tnp.second << ")\n";
#endif

    CollisionHandler ch = it->second;
    ch(pGo2, pGo1); // swapped
    return true;
  }
  
  return false;
}
}
