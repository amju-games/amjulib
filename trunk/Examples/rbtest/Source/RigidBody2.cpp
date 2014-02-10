#include <iostream>
#include "RigidBody2.h"
#include "RBManager.h"
#include <Vec3.h>

namespace Amju
{
RigidBody2::RigidBody2()
{
  m_invMass = 1.0f; 
  m_rads = 0;
  m_angVel = 0;
  m_torques = 0;
}

void RigidBody2::Update()
{
  float dt = TheRBManager::Instance()->GetDt();

  Vec2f acc = m_forces * m_invMass;
  m_forces = Vec2f();

  const Vec2f GRAVITY(0, -100.0f); // TODO TEMP TEST
  if (m_invMass > 0)
  {
    if (!IsResting())
    {
      acc += GRAVITY;
    }

#ifdef USE_VERLET

    float t2 = dt * dt;
    Vec2f diff = m_pos - m_oldPos + acc * t2;
    m_oldPos = m_pos;
    m_pos += diff;

#else

    Vec2f oldVel = m_vel;
    m_vel += acc * dt;
    m_pos += (oldVel + m_vel) * (0.5f * dt);

#endif
  }

/*
  if (fabs(m_torques) < 0.1f) // TODO
  {
    // Ignore small torques
    m_torques = 0;
  }
*/

  // TODO Separate moment of inertia
  float angAcc = m_torques * m_invMass; 
  m_torques = 0; 

  m_angVel += angAcc; // don;t depend on  dt : this is a one-off occurrence when a torque is applied

  // Damp rotation - TODO use dt
//  m_angVel *= 0.99f;

  m_rads += m_angVel * dt; 
}

void RigidBody2::AddForce(const Vec2f& force)
{
  m_forces += force;
}

void RigidBody2::AddTorque(const Vec2f& force, const Vec2f& poa) // pointOfApplication
{
  Vec3f force3(force.x, force.y, 0);
  Vec3f poa3(poa.x - m_pos.x, poa.y - m_pos.y, 0);
  Vec3f t = CrossProduct(force3, poa3);
  m_torques += t.z; 

//std::cout << "Adding torque: x: " << t.x << " y: " << t.y << " z: " << t.z << "\n";
}

void RigidBody2::SetPos(const Vec2f& pos)
{
  m_pos = pos;
}

void RigidBody2::SetVel(const Vec2f& v)
{
  m_vel = v;
}

void RigidBody2::SetRot(float rads)
{
  m_rads = rads;
}

void RigidBody2::SetAngVel(float radsPerSec)
{
  m_angVel = radsPerSec;
}

void RigidBody2::SetInvMass(float im)
{
  m_invMass = im;
}

}

