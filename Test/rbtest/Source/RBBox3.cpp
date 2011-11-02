#include "RBBox3.h"
#include "DrawOBB3.h"
#include "Timer.h"
#include <Sphere.h>
#include <DrawSphere.h>
#include <AmjuGL.h>
#include <iostream>
#include "RBManager.h"

namespace Amju
{
const RBBox3::TypeName RBBox3::TYPENAME = "rbbox3";

RBBox3::RBBox3()
{
  SetSize(Vec3f(1, 1, 1));
}

void RBBox3::SetSize(const Vec3f& s)
{
  m_obb3.SetExtents(s);
}

bool FindContact(const RBBox3& box1, const RBBox3& box2, Contact3* c)
{
  // Check for collision: SAT, ignores "tunnelling" for now, i.e. ignores velocities!
  if (!Intersects(box1.m_obb3, box2.m_obb3))
  {
    return false;
  }

  // Vertex of box1 may intersect a face of box2, or vice versa. So we call FindContact
  //  twice, swapping the parameters.
  if (box1.FindContact(box2, c))
  {
    return true;
  }

  if (box2.FindContact(box1, c))
  {
    return true;
  }

std::cout << "SAT says intersection between box3s, but no contact info!?!?!\n";
  return false;
}

bool RBBox3::FindContact(const RBBox3& b, Contact3* c) const
{
  // Find vertex-face contacts
  // For each vert, check if it intersects other box (behind all planes).
  // We collect contact normal and pen depth for all intersecting verts, then get the avg.
  // This is so if, say, two of our verts intersect the same face of b, we get the avg 
  // pen depth and avg contact normal.

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
    }
  }

  if (numPenetratingVerts > 0)
  {
    avgPd /= (float)numPenetratingVerts;
    // Get average of positions of penetrating verts, i.e. the centre of the points
    avgPos *= (1.0f / (float)numPenetratingVerts);
    contactNormal.Normalise();

    c->m_pos = avgPos;
    c->m_contactNormal = contactNormal;
    c->m_penetrationDepth = avgPd;
    return true;
  }
  
  return false;
}

void RBBox3::Update()
{
  RigidBody::Update();

  // Update OBB
  m_obb3.SetCentre(m_pos);
  m_obb3.SetOrientation(m_rot);
}

void RBBox3::Draw()
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
}

void RBBox3Collision(RBBox3* box1, RBBox3* box2)
{
  Contact3 c;
  if (FindContact(*box1, *box2, &c))
  {
std::cout << "Box3-box3 Contact!\n";

    // Got penetration. Move away in dir of contact normal.
    Vec3f contactNormal = c.m_contactNormal;

    // "Resolve" penetration: bodge it by moving away in direction of
    //  contact normal, by distance <penetration depth>
    // Don't call SetPos, this will zero pos diff for Verlet???

    // Move away, proportional to relative mass
    float totalInvMass = box1->m_invMass + box2->m_invMass;
    float frac1 = box1->m_invMass / totalInvMass;
    float frac2 = 1.0f - frac1;

    box1->m_pos += contactNormal * (c.m_penetrationDepth * frac1);
    box2->m_pos -= contactNormal * (c.m_penetrationDepth * frac2);

    // vel gets velChange added. If other object is immovable, v gets 2*velChange
    // and we get the reflection vector.
    // TODO!!!!
    Vec3f relvel = box1->m_vel - box2->m_vel;
    float relSpeed = sqrt(relvel.SqLen());

    float velChangeMult = DotProduct(box1->m_vel, contactNormal);
    Vec3f velChange = velChangeMult * contactNormal;
    // Immovable in this case TODO
    box1->m_vel -= 2.0f * velChange;

    // TODO TEMP TEST
    // Dampen the response - this happens once per collision, so not multiplied by dt etc
    box1->m_vel *= 0.5f;

    // Torque to apply? We know point of application. F = ma, a = vel change ?
    // Surely mag of torque should depend on vel of impact.

    // Mag of torque depends on how fast the contact point was moving
    //  relative to the face it penetrated.
    // i.e. Linear vel + rotational vel
    float mag = relSpeed * 0.4f; // TODO TEMP TEST
      //sqrt(m_vel.SqLen()) - crazy
      //velChangeMult - crazy;
      // * some fudge factor == force magnitude
    box1->AddTorque(mag * -contactNormal, c.m_pos);

    // This seems useless, as we are explicitly changing vel above, it doesn't
    //  make any difference -- except that it seems to have an effect on how
    //  things settle down.
    //AddForce(mag * contactNormal);
  }
}
static bool regRBBox3Collision = TheRBManager::Instance()->AddRBFunc(
  RBBox3::TYPENAME, RBBox3::TYPENAME, (RBManager::RBFunc)RBBox3Collision);

}
