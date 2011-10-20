#ifndef RB_MANAGER_H
#define RB_MANAGER_H

#include <vector>
#include <Singleton.h>
#include "RigidBody.h"

namespace Amju
{
class RBManager : public NonCopyable
{
public:
  void AddRB(RigidBody* rb);
  void Clear();

  void Update();
  void Draw();

private:
  typedef std::vector<PRigidBody> RBs;
  RBs m_rbs;
};

typedef Singleton<RBManager> TheRBManager;
}

#endif
