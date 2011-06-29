#ifndef RB_BOX_H
#define RB_BOX_H

#include "RigidBody.h"
#include "OBB3.h"

namespace Amju
{
class RBBox : public RigidBody
{
public:
  RBBox();
  virtual void Update();
  virtual void Draw();

protected:
  OBB3 m_obb3;
};
}

#endif
