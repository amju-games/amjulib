#ifndef RB_BOX_H
#define RB_BOX_H

#include "RigidBody.h"
#include <OBB3.h>

namespace Amju
{
class RBBox : public RigidBody
{
public:
  RBBox();
  virtual void Draw();
  virtual void Update();

protected:
  OBB3 m_obb;
};
}

#endif
