#include "RBBox.h"
#include "DrawOBB3.h"
#include "Timer.h"
#include <Sphere.h>
#include <DrawSphere.h>
#include <AmjuGL.h>

namespace Amju
{
RBBox::RBBox()
{
  SetSize(Vec3f(1, 1, 1));
}

void RBBox::SetSize(const Vec3f& s)
{
  m_obb3.SetExtents(s);
}

bool RBBox::FindContact(const RBBox& b, Contact* c) const
{
  // Check for collision: ignore "tunnelling" for now
  if (!Intersects(m_obb3, b.m_obb3))
  {
    return false;
  }

  // Find vertex-face contacts
  // For each vert, check if it intersects other box (behind all planes).
  Vec3f corners[8];
  m_obb3.GetCorners(corners);

  float avgPd = 0;
  Vec3f avgPos;
  int numPenetratingVerts = 0;
  Vec3f contactNormal;
  for (int i = 0; i < 8; i++)
  {
    float pd = 0;
    Vec3f cn;
    // Intersection test, also gets contact normal and penetration depth (i.e. dist behind plane) 
    if (b.m_obb3.Intersects(corners[i], &cn, &pd))
    {
      numPenetratingVerts++;
      avgPd += pd;
      contactNormal += cn;
      avgPos += corners[i];

      /*
      // Get most penetrating vert
      if (pd > bestPd)
      {
        bestPd = pd;
        penetratingVert = i;
        contactNormal = cn;
      }
      */
    }
  }

  avgPd /= (float)numPenetratingVerts;
  // Get average of positions of penetrating verts, i.e. the centre of the points
  avgPos *= (1.0f / (float)numPenetratingVerts);
  contactNormal.Normalise();

  if (numPenetratingVerts > 0)
  {
    c->m_pos = avgPos;
    c->m_contactNormal = contactNormal;
    c->m_penetrationDepth = avgPd;
    return true;
  }
  
  return false;
}

void RBBox::Update()
{
  RigidBody::Update();

  // Update OBB
  m_obb3.SetCentre(m_pos);
  m_obb3.SetOrientation(m_rot);

  /*
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
    // Got penetration. Move away in dir of contact normal.
    Vec3f contactNormal(0, 1, 0); // TODO just this case

    // "Resolve" penetration: bodge it by moving away in direction of
    //  contact normal, by distance <penetration depth>
    // Don't call SetPos, this will zero pos diff for Verlet???
    m_pos += contactNormal * bestPd; 

    // vel gets velChange added. If other object is immovable, v gets 2*velChange
    // and we get the reflection vector.
    float velChangeMult = DotProduct(m_vel, contactNormal);
    Vec3f velChange = velChangeMult * contactNormal;
    // Immovable in this case TODO
    m_vel -= 2.0f * velChange;

    // TODO TEMP TEST
    // Dampen the response - this happens once per collision, so not multiplied by dt etc
    m_vel *= 0.5f;

    // Torque to apply? We know point of application. F = ma, a = vel change ? 
    // Surely mag of torque should depend on vel of impact.
    float mag = 4.0f; //1.5f; // TODO TEMP TEST
      //sqrt(m_vel.SqLen()) - crazy
      //velChangeMult - crazy; 
      // * some fudge factor == force magnitude
    AddTorque(mag * contactNormal, corners[penetratingVert]);

    // This seems useless, as we are explicitly changing vel above, it doesn't
    //  make any difference -- except that it seems to have an effect on how
    //  things settle down.
    //AddForce(mag * contactNormal);
  }
  */
}

void RBBox::Draw()
{
  DrawOBB3(m_obb3);

  Vec3f corners[8];
  m_obb3.GetCorners(corners);

  const float RADIUS = 0.1f;

  for (int i = 0; i < 8; i++)
  { 
    PushColour();
    if (0)
    {
      MultColour(Colour(1, 0, 0, 1));
    }
    Sphere s(corners[i], RADIUS);
    DrawSphere(s);
    PopColour();
  }

  //std::cout << "Pos: " << m_pos.x << ", " << m_pos.y << ", " << m_pos.z << "\n";
}

}
