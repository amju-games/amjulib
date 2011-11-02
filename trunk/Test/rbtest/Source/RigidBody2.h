#ifndef RIGID_BODY_2_H
#define RIGID_BODY_2_H

#include <Vec2.h>
#include "RB.h"

namespace Amju
{
class RigidBody2 : public RB
{
public:
  RigidBody2();
  virtual ~RigidBody2() {}

  virtual void Draw() = 0;
  virtual void Update();

  void AddForce(const Vec2f& force);
  void AddTorque(const Vec2f& force, const Vec2f& pointOfApplication);

  const Vec2f& GetPos() const { return m_pos; }
  const Vec2f& GetVel() const { return m_vel; }

  void SetPos(const Vec2f& pos);
  void SetVel(const Vec2f& pos);
  void SetRot(float rads);
  void SetAngVel(float radsPerSec);
  void SetInvMass(float im);

protected:
  float m_invMass;
  Vec2f m_pos;
  Vec2f m_oldPos; // Verlet
  Vec2f m_vel; // Euler

  Vec2f m_forces;
  float m_torques;
  float m_rads; // angle (rads) about z axis
  float m_angVel; // ??
};
}

#endif

