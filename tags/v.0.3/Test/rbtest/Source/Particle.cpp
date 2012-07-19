#include "Particle.h"
#include <Sphere.h>
#include <DrawSphere.h>
#include <Colour.h>
#include "RBManager.h"

namespace Amju
{
static const float GRAVITY = -2.0f;

Particle::Particle()
{
  m_invMass = 1.0f;
  ResetForces();
}

void Particle::SetInvMass(float invMass)
{
  m_invMass = invMass;
}

void Particle::Draw()
{
  const float RADIUS = 0.1f;
  PushColour();
  if (m_invMass <= 0)
  {
      MultColour(Colour(1, 0, 0, 1));
  }
  Sphere s(m_pos, RADIUS);
  DrawSphere(s);
  PopColour();
}

void Particle::Move(const Vec3f& v)
{
  if (m_invMass > 0)
  {
      m_pos += v;
  }
}

void Particle::Update()
{
  m_acc = m_forces * m_invMass;
  if (m_invMass > 0)
  {
    m_acc += Vec3f(0, GRAVITY, 0);
  }

  float dt = TheRBManager::Instance()->GetDt();
  Vec3f oldVel = m_vel;
  m_vel += m_acc * dt;

  // Damping -- TODO, is dependent on scene complexity
  //m_vel *= (1.0f - dt * 2.0f); 

  Vec3f pos = m_pos + (m_vel + oldVel) * 0.5f * dt;
  SetPos(pos);

  ResetForces();

  // TODO TEMP TEST
  //if (m_pos.x > -2 && m_pos.x < 2 && m_pos.z > -2 && m_pos.z < 2 && m_pos.y < -2)
  /*if (m_pos.x > 0 && m_pos.z > 0 && m_pos.y < -2)
  {
      m_pos.y = -2;
  }*/


}

void Particle::AddForce(const Vec3f& force)
{
  m_forces += force;
}

void Particle::ResetForces()
{
  // Only force is due to gravity
  m_forces = Vec3f(0, 0, 0);
}

void Particle::SetPos(const Vec3f& pos)
{
  m_pos = pos;
}

const Vec3f& Particle::GetPos() const
{
  return m_pos;
}
}
