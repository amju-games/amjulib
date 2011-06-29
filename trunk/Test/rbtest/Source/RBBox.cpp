#include "RBBox.h"
#include "DrawOBB3.h"
#include "Timer.h"

namespace Amju
{
RBBox::RBBox()
{
  m_obb3.SetExtents(Vec3f(2, 1, 1));
}

void RBBox::Update()
{
  RigidBody::Update();

  // Update OBB
  m_obb3.SetCentre(m_pos);
  m_obb3.SetOrientation(m_rot);

  // TODO TEMP TEST
  // Test box corners to see if we have gone through the floor. 
  // TODO Generalise to box-plane collision 
  Vec3f corners[8];
  m_obb3.GetCorners(corners);
  float bestPd = 0;
  int penetratingVert = -1;
  for (int i = 0; i < 8; i++)
  {
    if (corners[i].y < 0)
    {
      // Get penetration depth (i.e. dist behind plane)
      float pd = -corners[i].y;
      // Get most penetrating vert
      if (pd > bestPd)
      {
        bestPd = pd;
        penetratingVert = i;
      }
    }
  }

  float dt = TheTimer::Instance()->GetDt();

  if (penetratingVert > -1)
  {
    // Got penetration. Move away in dir of plane normal.

    // Don't call SetPos, this will zero pos diff for Verlet.
    m_pos.y += bestPd; // plane normal * bestPd

    // vel gets velChange added. If other object is immovable, v gets 2*velChange
    // and we get the reflection vector.
    Vec3f contactNormal(0, 1, 0); // TODO just this case
    Vec3f velChange = DotProduct(m_vel, contactNormal) * contactNormal;
    // Immovable in this case TODO
    m_vel -= 2.0f * velChange;

    // Torque to apply? We know point of application. F = ma, a = vel change ? 
    float mag = 2.0f; // force magnitude
    AddTorque(mag * contactNormal, corners[penetratingVert]);
  }
}

void RBBox::Draw()
{
  DrawOBB3(m_obb3);
  //std::cout << "Pos: " << m_pos.x << ", " << m_pos.y << ", " << m_pos.z << "\n";
}

}