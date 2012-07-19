#ifndef RB_SCENE_NODE_H
#define RB_SCENE_NODE_H

#include <SceneNode.h>
#include "RB.h"

namespace Amju
{
class RBSceneNode : public SceneNode
{
public:
  RBSceneNode(RB* rb = 0) : m_rb(rb) {}

  virtual void Update();
  virtual void Draw();

  void SetRB(RB* rb) { m_rb = rb; }

private:
  RB* m_rb;
};

}

#endif

