#ifndef RB_BOX_3_H
#define RB_BOX_3_H

#include "RigidBody3.h"
#include "OBB3.h"
#include "Contact.h"

namespace Amju
{
class RBBox3 : public RigidBody
{
public:
  RBBox3();
  void SetSize(const Vec3f&);

  virtual void Update();
  virtual void Draw();
  virtual TypeName GetTypeName() const { return TYPENAME; }
  static const TypeName TYPENAME;

protected:
  OBB3 m_obb3;

  // Checks for collision between two boxes. If found, moves the boxes away from each other
  // and applies impulse and torque.
  friend void RBBox3Collision(RBBox3* box1, RBBox3* box2);
 
  // Non-member function, so public 
  // Find contact between two boxes
  friend bool FindContact(const RBBox3& box1, const RBBox3& box2, Contact3* c);

  // Above fuction calls this private member function, which does half of the job
  bool FindContact(const RBBox3& b, Contact3* c) const;
};
}

#endif
