#ifndef SAP_H_INCLUDED
#define SAP_H_INCLUDED

// Sweep and Prune implementation
// Adapted from Art Of Collision tutorial
// http://www.shmup-dev.com/forum/index.php?page=24

#include <set>
#include <GameObject.h>
#include "CollisionManager.h"

namespace Amju
{
class SweepAndPrune : public CollisionDetector
{
public:
  SweepAndPrune();

  virtual void Update() override;
  virtual void Clear() override;

  // For dynamically adding/removing game objects.
  // TODO Do this automatically in Update
  int AddBox(GameObject* go); 
  // TODO
  //void RemoveBox(GameObject* go); 

private:
  typedef std::pair<PGameObject, PGameObject> Encounter;

  enum { AMJU_SAP_AXES = 3 };
  enum { AMJU_SAP_MAX_BOXES = 1000 };

  struct Endpoint
  {
    enum Type { AMJU_SAP_MIN, AMJU_SAP_MAX };
    Type m_type;
    PGameObject m_go;
  };

  typedef std::set<Encounter> EncounterSet;
  EncounterSet m_encounters;

  Endpoint m_endpoints[2 * AMJU_SAP_MAX_BOXES][AMJU_SAP_AXES]; 
  // our endpoint arrays > 2 endpoints per box per axis

  int m_numBoxes; // current number of boxes (== Game Objects) we have

private:
  void UpdateInternal();

  void ResolveEncounters(); 

  void AddEncounter(GameObject* go1, GameObject* go2);
  void RemoveEncounter(GameObject* go1, GameObject* go2);

}; // class SAP

} // namespace 

#endif

