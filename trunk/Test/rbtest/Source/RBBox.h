#ifndef RB_BOX_H
#define RB_BOX_H

#include "RigidBody.h"
#include "OBB3.h"
#include "Contact.h"

namespace Amju
{
class RBBox : public RigidBody
{
public:
  RBBox();
  void SetSize(const Vec3f&);

  virtual void Update();
  virtual void Draw();

  bool FindContact(const RBBox& b, Contact* c) const;

protected:
  OBB3 m_obb3;
};
}

#endif
