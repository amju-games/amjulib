#include "RBManager.h"
#include "RBBox.h" // TODO TEMP TEST
#include "FindContact.h"

namespace Amju
{
void RBManager::AddRB(RigidBody* rb)
{
  m_rbs.push_back(rb);
}

void RBManager::Clear()
{
  m_rbs.clear();
}

void RBManager::Update()
{
  for (RBs::iterator it = m_rbs.begin(); it != m_rbs.end(); ++it)
  {
    (*it)->Update();
  }

  // Collisions - TODO use SAP
  for (RBs::iterator it = m_rbs.begin(); it != m_rbs.end(); ++it)
  {
    for (RBs::iterator jt = it + 1; jt != m_rbs.end(); ++jt)
    {
      RigidBody* rb1 = *it;
      RigidBody* rb2 = *jt;

      // TODO Use double dipatcher

      RBBox* box1 = dynamic_cast<RBBox*>(rb1);
      RBBox* box2 = dynamic_cast<RBBox*>(rb2);

      Contact c;
      if (FindContact(*box1, *box2, &c))
      {
        // Respond - TODO put in handler function

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
        float mag = relSpeed * 0.1f; // TODO TEMP TEST
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
  }
}

void RBManager::Draw()
{
  for (RBs::iterator it = m_rbs.begin(); it != m_rbs.end(); ++it)
  {
    (*it)->Draw();
  }
}
}
