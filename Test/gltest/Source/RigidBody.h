#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <Vec3.h>
//#include <vector>
#include <RCPtr.h>
#include <Quaternion.h>

namespace Amju
{
class RigidBody : public RefCounted
{
public:
  RigidBody();
  virtual ~RigidBody() {}

  virtual void Draw();
  virtual void Update();

  // Force causing linear motion only, e.g. gravity
  void AddForce(const Vec3f& force);

  // Force also potentially causing rotation
  void AddForce(const Vec3f& force, const Vec3f& pos);

  void ResetForces(); // Called at end of Update anyway

  void SetPos(const Vec3f& pos);
  const Vec3f& GetPos() const;

  void SetInvMass(float invMass);

protected:
  Vec3f m_pos;
  Vec3f m_vel;
  Vec3f m_acc;
  float m_invMass;
  Vec3f m_forces; // accumulated forces added together (D'Alembert)
  Vec3f m_torques; // accumulated torques (aka moments)

  Quaternion m_quat; // orientation
  Quaternion m_angVel; // ??
  Quaternion m_angAcc; // ???
};

typedef RCPtr<RigidBody> PRigidBody;
}

#endif
