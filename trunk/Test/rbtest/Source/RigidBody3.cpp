#include "RigidBody3.h"
#include "RBManager.h"

//#define USE_VERLET

namespace Amju
{
RigidBody::RigidBody()
{
  m_invMass = 1.0f;
}

void RigidBody::SetInvMass(float im)
{
  m_invMass = im;
}

void RigidBody::Update()
{
  float dt = TheRBManager::Instance()->GetDt();

  Vec3f acc = m_forces * m_invMass;

  const Vec3f GRAVITY(0, -50.0f, 0); // TODO TEMP TEST
  if (m_invMass > 0)
  {
    acc += GRAVITY;
  }

  m_forces = Vec3f();

#ifdef USE_VERLET

  float t2 = dt * dt;
  Vec3f diff = m_pos - m_oldPos + acc * t2;
  m_oldPos = m_pos;
  m_pos += diff;

#else

  Vec3f oldVel = m_vel;
  m_vel += acc * dt;
  m_pos += (oldVel + m_vel) * (0.5f * dt); 

#endif

  Vec3f angAcc = m_torques; //  * momInertia
  m_torques = Vec3f();

  m_angVel += angAcc * dt;
  if (m_angVel.SqLen() > 0.00001f)
  {
    Vec3f axis = m_angVel;
    axis.Normalise();
    Quaternion q(axis, m_angVel.SqLen());
    m_rot = q * m_rot;
    m_rot.Normalize();
  }
}

void RigidBody::AddForce(const Vec3f& force)
{
  m_forces += force;
}

void RigidBody::AddTorque(const Vec3f& force, const Vec3f& pointOfApplication)
{
  Vec3f t = CrossProduct(force, pointOfApplication - m_pos);
  m_torques += t;
}

void RigidBody::SetPos(const Vec3f& pos)
{
  m_pos = pos;
  m_oldPos = pos; // ?
}

void RigidBody::SetRot(const Quaternion& q)
{
  m_rot = q;
}
}
