#ifndef RB_SPHERE_H
#define RB_SPHERE_H

#include "RigidBody.h"
#include "Sphere.h"

namespace Amju
{
class RBSphere : public RigidBody
{
public:
  RBSphere();
  virtual void Update();
  virtual void Draw();

  float GetRadius() const { return m_sphere.GetRadius(); }
  void SetRadius(float r) { m_sphere.SetRadius(r); }

private:
  Sphere m_sphere;
};
}

#endif
