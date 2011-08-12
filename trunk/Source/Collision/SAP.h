#ifndef SAP_H_INCLUDED
#define SAP_H_INCLUDED

// Sweep and Prune implementation
// Adapted from Art Of Collision tutorial
// http://www.shmup-dev.com/forum/index.php?page=24

#include <set>
#include <GameObject.h>
#include <Singleton.h>

namespace Amju
{
typedef bool (*CollideFunc)(GameObject*, GameObject*);

class SweepAndPrune
{
  enum { AMJU_SAP_AXES = 3 };
  enum { AMJU_SAP_MAX_BOXES = 1000 };
  
  SweepAndPrune();
  friend class Singleton<SweepAndPrune>;

public:
  void Update(const GameObjects& gos);
  void Clear();
  void SetCollideFunc(CollideFunc cf);

private:
  typedef std::pair<GameObject*, GameObject*> Encounter;

  struct Endpoint
  {
    enum Type { AMJU_SAP_MIN, AMJU_SAP_MAX };
    Type m_type;
    GameObject* m_go;
  };

  typedef std::set<Encounter> EncounterSet;
  EncounterSet m_encounters;

  Endpoint m_endpoints[2 * AMJU_SAP_MAX_BOXES][AMJU_SAP_AXES]; 
  // our endpoint arrays > 2 endpoints per box per axis

  int m_numBoxes; // current number of boxes (== Game Objects) we have

  CollideFunc m_collideFunc;

private:
  void Update();

  void ResolveEncounters(); 

  void AddEncounter(GameObject* go1, GameObject* go2);
  void RemoveEncounter(GameObject* go1, GameObject* go2);

  int AddBox(GameObject* go); 

  // TODO
  //void RemoveBox(GameObject* go); 

}; // class SAP

typedef Singleton<SweepAndPrune> TheSAP;

} // namespace 

#endif

