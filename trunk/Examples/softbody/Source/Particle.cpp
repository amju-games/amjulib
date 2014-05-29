#include <Sphere.h>
#include <DrawSphere.h>
#include <Colour.h>
#include <Timer.h>
#include "Particle.h"

#ifdef AMJU_USE_GLUT
#ifdef WIN32
#include <gl/glut.h>
#endif
#ifdef MACOSX
#include <GLUT/glut.h>
#endif
#endif

namespace Amju
{
Vec3f Particle::GRAVITY;

float Particle::DRAW_RADIUS = 0.5f; 

void Particle::SetDrawRadius(float r)
{
  DRAW_RADIUS = r;
}

void Particle::SetGravity(const Vec3f& g)
{
  GRAVITY = g;
}

Particle::Particle(int id)
{
  m_id = id;
  m_invMass = 1.0f;
  ResetForces();
}

int Particle::GetId() const
{
  return m_id;
}

void Particle::SetInvMass(float invMass)
{
  m_invMass = invMass;
}

void Particle::Draw()
{
  PushColour();
  if (m_invMass <= 0)
  {
      MultColour(Colour(1, 0, 0, 1));
  }

#ifdef AMJU_USE_GLUT
  glPushMatrix();
  glTranslatef(m_pos.x, m_pos.y, m_pos.z);
  glutSolidSphere(DRAW_RADIUS, 8, 8);
  glPopMatrix();
#else
  Sphere s(m_pos, DRAW_RADIUS);
  DrawSphere(s);
#endif
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
    m_acc += GRAVITY;
  }

  float dt = TheTimer::Instance()->GetDt();
  Vec3f oldVel = m_vel;
  m_vel += m_acc * dt;

  // Damping -- TODO, is dependent on scene complexity
  //m_vel *= (1.0f - dt * 2.0f); 
  m_vel *= 0.99f; // TODO

  Vec3f pos = m_pos + (m_vel + oldVel) * 0.5f * dt;
  SetPos(pos);

  ResetForces();

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
