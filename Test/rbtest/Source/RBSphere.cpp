#include "RBSphere.h"
#include "DrawSphere.h"

namespace Amju
{
RBSphere::RBSphere()
{
  m_sphere.SetRadius(1.0f); // TODO TEMP TEST
}

void RBSphere::Update()
{
  RigidBody::Update();
  m_sphere.SetCentre(m_pos);

  // TEST - collision with plane
  if (m_pos.y - m_sphere.GetRadius() < 0)
  {
    // Move back by penetration depth
    // OR calc response at time of first contact
    float pd = m_sphere.GetRadius() - m_pos.y; // TODO
    m_pos.y += pd; // move away by pen depth

    // vel gets velChange added. If other object is immovable, v gets 2*velChange
    // and we get the reflection vector.
    Vec3f contactNormal(0, 1, 0); // TODO just this case
    Vec3f velChange = DotProduct(m_vel, contactNormal) * contactNormal;
    // Immovable in this case TODO
    m_vel -= 2.0f * velChange;
  }
}

void RBSphere::Draw()
{
  DrawSphere(m_sphere);
}
}
