#ifndef RB_BOX_2_H
#define RB_BOX_2_H

#include "RigidBody2.h"
#include "OBB2.h"
#include "Contact.h"

namespace Amju
{
class RBBox2 : public RigidBody2
{
public:
  RBBox2();
  void SetSize(const Vec2f&);

  virtual void Update();
  virtual void Draw();
  virtual TypeName GetTypeName() const { return TYPENAME; }
  static const TypeName TYPENAME;

protected:
  OBB2 m_obb2;

  // Checks for collision between two boxes. If found, moves the boxes away from each other
  // and applies impulse and torque.
  friend void RBBox2Collision(RBBox2* box1, RBBox2* box2);

  friend bool FindContact(const RBBox2& box1, const RBBox2& box2, Contact2* c);
  
  // Check for collision between this box and the other box, b. If they collide, contact info c
  //  is populated and we return true. Returns false if no collision.
  bool FindContact(const RBBox2& b, Contact2* c) const;

  // To draw contact info
  Contact2 m_contact; 
  Vec2f m_torqueArrow[2]; // to draw arrow showing last torque set
};
}

#endif
