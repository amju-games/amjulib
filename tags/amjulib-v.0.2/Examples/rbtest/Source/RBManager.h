#ifndef RB_MANAGER_H
#define RB_MANAGER_H

#include <map>
#include <vector>
#include <Singleton.h>

//#include <DoubleDisp.h>
// Can't use Double Disp!

#include "RB.h"

namespace Amju
{
class RBManager : public NonCopyable
{
public:
  void AddRB(RB* rb);
  void Clear();

  void Update();
  void Draw();

  // Collision test/resolve function
  typedef void (*RBFunc)(RB*, RB*);

  // Add a handler function
  bool AddRBFunc(RB::TypeName, RB::TypeName, RBFunc);

  // RBs should use this function to get dt, not TheTimer.
  // TODO Maybe rewind to previous frame and restart with a different dt value so we always
  //  resolve collisions in the order they would happen...
  float GetDt() const; 

private:
  typedef std::vector<RCPtr<RB> > RBs;
  RBs m_rbs;

  typedef std::pair<RB::TypeName, RB::TypeName> TNPair;
  typedef std::map<TNPair, RBFunc> RBMap;
  RBMap m_map;

private:
  bool Dispatch(RB* rb1, RB* rb2);
};

typedef Singleton<RBManager> TheRBManager;
}

#endif
