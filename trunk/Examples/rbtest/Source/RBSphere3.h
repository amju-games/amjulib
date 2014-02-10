#ifndef RB_SPHERE_3_H
#define RB_SPHERE_3_H

#include "RigidBody3.h"
#include "Sphere.h"

namespace Amju
{
class RBSphere3 : public RigidBody
{
public:
  RBSphere3();
  virtual void Update();
  virtual void Draw();
  virtual TypeName GetTypeName() const { return TYPENAME; }
  static const TypeName TYPENAME;

  float GetRadius() const { return m_sphere.GetRadius(); }
  void SetRadius(float r) { m_sphere.SetRadius(r); }

private:
  Sphere m_sphere;
};
}

#endif
