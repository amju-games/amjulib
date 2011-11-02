#ifndef RB_SPHERE_2_H
#define RB_SPHERE_2_H

#include "RigidBody2.h"

namespace Amju
{
class RBSphere2 : public RigidBody2
{
public:
  RBSphere2();
  virtual void Update();
  virtual void Draw();
  virtual TypeName GetTypeName() const { return TYPENAME; }
  static const TypeName TYPENAME;

  float GetRadius() const { return m_radius; }
  void SetRadius(float r) { m_radius = r; }

private:
  float m_radius;
};
}

#endif

