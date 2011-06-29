#ifndef AMJU_RIGID_BODY_H
#define AMJU_RIGID_BODY_H

#include "Vec3.h"
#include "Quaternion.h"

namespace Amju
{
class RigidBody
{
public:
  RigidBody();
  virtual ~RigidBody() {}
  virtual void Update();

  void AddForce(const Vec3f& force);
  void AddTorque(const Vec3f& force, const Vec3f& pointOfApplication);

  const Vec3f& GetPos() const { return m_pos; }
  const Vec3f& GetVel() const { return m_vel; }

  void SetPos(const Vec3f& pos);
  void SetRot(const Quaternion&);

protected:
  float m_invMass;
  Vec3f m_pos; // position and centre of mass
  Vec3f m_oldPos; // for Verlet integration
  Vec3f m_vel; // for Euler integration

  Vec3f m_forces; // sum of linear forces
  Vec3f m_torques; // sum of torques
  Quaternion m_rot;
  Vec3f m_angVel; // dir is axis of rotation, magnitude is rot speed
};
}

#endif
