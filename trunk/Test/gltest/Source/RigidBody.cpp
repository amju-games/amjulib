#include "RigidBody.h"
#include <Timer.h>

namespace Amju
{
static const float GRAVITY = 0;

RigidBody::RigidBody()
{
  m_invMass = 1.0f;
  ResetForces();
}

void RigidBody::Draw()
{
}

void RigidBody::Update()
{
  m_acc = m_forces * m_invMass;
  if (m_invMass > 0)
  {
    m_acc += Vec3f(0, GRAVITY, 0);
  }

  float dt = TheTimer::Instance()->GetDt();
  Vec3f oldVel = m_vel;
  m_vel += m_acc * dt;

  // Damping -- TODO, is dependent on scene complexity
  m_vel *= (1.0f - dt * 2.0f); 

  Vec3f pos = m_pos + (m_vel + oldVel) * 0.5f * dt;
  SetPos(pos);

  // Accumulated torque gives rotational acceleration

  if (m_torques.SqLen() > 0)
  {
    // ang accel = torque over moment of inertia. 
    Vec3f angAccAxis = m_torques; // assume 1 m_inverseInertiaTensor * m_torques;
    angAccAxis.Normalise();
    // Magnitude of ang accel ? What are units ??
    float angAccMag = sqrt(m_torques.SqLen());
    m_angAcc.SetAxisAngle(angAccMag, angAccAxis);
  
    // Apply new angular accel to ang vel
    m_angVel = Quaternion::Slerp(Quaternion(), m_angAcc, dt) * m_angVel;
  }

  // Apply angular velocity to orientation (a quat)
  // Ang vel is rotation per second to apply to orientation

  m_quat = Quaternion::Slerp(Quaternion(), m_angVel, dt) * m_quat;

  ResetForces();
}

void RigidBody::AddForce(const Vec3f& force)
{
  // Linear only
  m_forces += force;
}

void RigidBody::AddForce(const Vec3f& force, const Vec3f& pos)
{
  // Force causing linear accel and torque
  Vec3f torque = CrossProduct(force, (pos - m_pos));
  m_torques += torque;

  m_forces += force;
}

void RigidBody::ResetForces()
{
  m_forces = Vec3f(0, 0, 0);
  m_torques = Vec3f(0, 0, 0);
}

void RigidBody::SetPos(const Vec3f& pos)
{
  m_pos = pos;
}

const Vec3f& RigidBody::GetPos() const
{
  return m_pos;
}

void RigidBody::SetInvMass(float invMass)
{
  m_invMass = invMass;
}
}
