#include "Spring.h"
#include "Particle.h"
#include <AmjuGL.h>
#include <DrawOBB3.h>

namespace Amju
{
static const float MAX_LEN = 10.0f; // Default Max length for which Hooke holds
static const float K = 1.0f; // Default spring constant

Spring::Spring()
{
  m_particles[0] = 0;
  m_particles[1] = 0;

  m_maxLength = MAX_LEN;
  m_minLength = 0;
  m_k = K;
}

Spring::Spring(Particle* p1, Particle* p2)
{
  ConnectToParticles(p1, p2);

  m_maxLength = MAX_LEN;
  m_minLength = 0;
  m_k = K;
}

void Spring::ConnectToParticles(Particle* p1, Particle* p2)
{
  m_particles[0] = p1;
  m_particles[1] = p2;
  m_oldVec = m_particles[0]->GetPos() - m_particles[1]->GetPos();
  m_naturalLength = sqrt(m_oldVec.SqLen());
}


void Spring::SetNaturalLength(float len)
{
    m_naturalLength = len;
}

void Spring::SetMaxLength(float maxLen)
{
    m_maxLength = maxLen;
}

void Spring::SetMinLength(float minLen)
{
    m_minLength = minLen;
}

void Spring::SetK(float k)
{
    m_k = k;
}

void Spring::SetQuat(const Quaternion& q)
{
  m_quat = q;
  m_box.SetOrientation(q);
}

const Quaternion& Spring::GetQuat() const
{
  return m_quat;
}

void Spring::Update()
{
  // Vector between spring ends
  Vec3f v = m_particles[0]->GetPos() - m_particles[1]->GetPos();
  Vec3f newVec = v;
  // Force along this line proportional to distance
  // == Hooke's Law, springs
  float len = sqrt(v.SqLen());
  //len = std::min(m_maxLength, len);
  v.Normalise();

  // Force at spring ends is proportional to squash/stretch distance
  Vec3f forceAtEnd = v * ((len - m_naturalLength) * m_k);
  m_particles[0]->AddForce(-forceAtEnd);
  m_particles[1]->AddForce(forceAtEnd);

  // If max spring length exceeded, pull ends together
  if (len > m_maxLength)
  {
      float dist = (len - m_maxLength) * 0.5f; // move each end this dist
      Vec3f moveVec = v * dist;
      m_particles[0]->Move(-moveVec);
      m_particles[1]->Move(moveVec);
  }
  else if (len < m_minLength)
  {
      float dist = (len - m_minLength) * 0.5f; // move each end this dist
      Vec3f moveVec = v * dist;
      m_particles[0]->Move(-moveVec);
      m_particles[1]->Move(moveVec);
  }


  // Set box centre and extents
  Vec3f centre = (m_particles[0]->GetPos() + m_particles[1]->GetPos()) * 0.5f;
  m_box.SetCentre(centre);
  m_box.SetExtents(Vec3f(len * 0.5f, 0.2f, 1.0f));

  // Align box with line seg connecting end points
  Vec3f xAxis = v;
  // Create a zAxis perpendicular to xAxis
  Vec3f up(0, 1.0f, 0);
  // Choose new up vector if xAxis is vertical!
  const float VERTICAL = 0.9f;
  float dot = DotProduct(xAxis, up);
  if (fabs(dot) > VERTICAL)
  {
    up = Vec3f(1.0f, 0, 0); // TODO may face other dir
  }
  Vec3f zAxis = CrossProduct(xAxis, up);
  Vec3f yAxis = CrossProduct(xAxis, zAxis);
  m_box.SetAxes(xAxis, yAxis);

/*
  // Twist spring
  m_oldVec.Normalise();
  newVec.Normalise();
  float theta = acos(DotProduct(m_oldVec, newVec));
  if (fabs(theta) > 0.001f)
  {
    Vec3f axis = CrossProduct(m_oldVec, newVec);
    axis.Normalise();
    Quaternion q(axis, theta);
    q = GetQuat() * q;
    SetQuat(q);
  }
*/
  m_oldVec = newVec;
}

void Spring::Draw()
{
  // Draw OBB between the 2 particles..?
  // Spring can be twisted ??

  // Just a line for now
  Vec3f p1 = m_particles[0]->GetPos();
  Vec3f p2 = m_particles[1]->GetPos();
  AmjuGL::DrawLine(AmjuGL::Vec3(p1.x, p1.y, p1.z), AmjuGL::Vec3(p2.x, p2.y, p2.z));

  DrawOBB3(m_box);
}
}
