#include "RBBox2.h"
#include "DrawOBB2.h"
#include "Timer.h"
#include <AmjuGL.h>
#include <iostream>
#include "RBManager.h"
#include <Pause.h>

namespace Amju
{
const RBBox2::TypeName RBBox2::TYPENAME = "rbbox2";

RBBox2::RBBox2()
{
  SetSize(Vec2f(1, 1));
}

void RBBox2::SetSize(const Vec2f& s)
{
  m_obb2.SetExtents(s);
}

bool FindContact(const RBBox2& box1, const RBBox2& box2, Contact2* c)
{
  // Check for collision: ignore "tunnelling" for now
  if (!Intersects(box1.m_obb2, box2.m_obb2))
  {
    return false;
  }

  Contact2 c1, c2;
  // Vert(s) from box1 inside box2 ?
  bool box1InBox2 = box1.FindContact(box2, &c1);

  // Vert(s) from box2 inside box1 ?
  bool box2InBox1 = box2.FindContact(box1, &c2);

  if (box1InBox2 && box2InBox1)
  {
    // Intersecting each other. Get avg contact point. Make contact normal always w.r.t. box2
    *c = c1;
    c->m_penetrationDepth = std::max(c1.m_penetrationDepth, c2.m_penetrationDepth); // so we detach fully
    c->m_pos = (c1.m_pos + c2.m_pos) * 0.5f; // mid point of contact points
    return true;
  }
  else if (box1InBox2)
  {
    *c = c1;
    return true;
  }
  else if (box2InBox1)
  {
    *c = c2;
    c->m_contactNormal = -c->m_contactNormal;
    return true;
  }

// TODO Come back to this. SAT is giving false positives (all the frickin' time??!?)
//std::cout << "SAT: box2 collision! But no contact info!?\n";
  return false;
}

bool RBBox2::FindContact(const RBBox2& b, Contact2* c) const
{
  // Find vertex-face contacts
  // For each vert, check if it intersects other box (behind all planes).
  Vec2f corners[4];
  m_obb2.GetCorners(corners);

  float avgPd = 0;
  Vec2f avgPos;
  int numPenetratingVerts = 0;
  Vec2f contactNormal;
  for (int i = 0; i < 4; i++)
  {
    float pd = 0;
    Vec2f cn;
    // Intersection test, also gets contact normal and penetration depth (i.e. dist behind plane) 
    if (b.m_obb2.Intersects(corners[i], &cn, &pd))
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

void RBBox2::Update()
{
  RigidBody2::Update();

  // Update OBB
  m_obb2.SetCentre(m_pos);
  m_obb2.SetRotation(m_rads);
}

void RBBox2::Draw()
{
  DrawOBB2(m_obb2);

/*
  Vec2f p = m_contact.m_pos;
  Vec2f q = p + m_contact.m_contactNormal;
  AmjuGL::DrawLine(AmjuGL::Vec3(p.x, p.y, 0), AmjuGL::Vec3(q.x, q.y, 0));
*/

  // Draw arrow showing last torque applied
  Vec2f t0 = m_torqueArrow[0];
  Vec2f t1 = m_torqueArrow[1];
  PushColour();
  MultColour(Colour(1, 0, 1, 1));
  AmjuGL::DrawLine(AmjuGL::Vec3(t0.x, t0.y, 0), AmjuGL::Vec3(t1.x, t1.y, 0));
  PopColour();


/*
  Vec3f corners[4];
  m_obb2.GetCorners(corners);

  const float RADIUS = 0.1f;

  for (int i = 0; i < 8; i++)
  { 
    PushColour();
    if (0)
    {
      MultColour(Colour(1, 0, 0, 1));
    }
    Sphere s(Vec3f(corners[i].x, corners[i].y, 0), RADIUS);
    DrawSphere(s);
    PopColour();
  }
*/
}

void RBBox2Collision(RBBox2* box1, RBBox2* box2)
{
      Contact2 c;
      if (FindContact(*box1, *box2, &c))
      {
std::cout << "Contact! box " << box1->GetId() << " and box " << box2->GetId() << "\n";

        // Contact normal should always be pointing from contact pos away/out of box 1
        // Got penetration. Move away in dir of contact normal.
        Vec2f contactNormal = c.m_contactNormal;
std::cout << "Contact normal: x: " << contactNormal.x << " y: " << contactNormal.y << "\n";
        
        // Store contact info to draw it
        box1->m_contact = c;

        // "Resolve" penetration: bodge it by moving away in direction of
        //  contact normal, by distance <penetration depth>
        // Don't call SetPos, this will zero pos diff for Verlet???

        // Move away, proportional to relative mass
        float totalInvMass = box1->m_invMass + box2->m_invMass;
        float frac1 = box1->m_invMass / totalInvMass;
        float frac2 = 1.0f - frac1;

std::cout << "Pen depth: " << c.m_penetrationDepth << "\n";
std::cout << "Frac1: " << frac1 << "\n";
std::cout << "Frac2: " << frac2 << "\n";

        const float TINY_BIT = 0.01f;
        float pd = c.m_penetrationDepth + TINY_BIT;
        box1->m_pos += contactNormal * (pd * frac1);
        box2->m_pos -= contactNormal * (pd * frac2);

std::cout << "Box 1 vel: " << box1->m_vel.x << ", " << box1->m_vel.y << "\n";
std::cout << "Box 2 vel: " << box2->m_vel.x << ", " << box2->m_vel.y << "\n";

        Vec2f relvel = box1->m_vel - box2->m_vel;
        float relSpeed = sqrt(relvel.SqLen());

        // Get rotational speed of contact point on box 1
        float cpdist1 = sqrt((c.m_pos - box1->m_pos).SqLen()); // dist from centre of box 1 to contact point
        // Speed (?) of this point is angVel * dist
        float speed1 = box1->m_angVel * cpdist1;
std::cout << "Box 1 ang vel: " << box1->m_angVel << " cpdist1: " << cpdist1 << " speed1: " << speed1 << "\n";

        float cpdist2 = sqrt((c.m_pos - box2->m_pos).SqLen()); // dist from centre of box 2 to contact point
        float speed2 = box2->m_angVel * cpdist2;
std::cout << "Box 2 ang vel: " << box2->m_angVel << " cpdist2: " << cpdist2 << " speed2: " << speed2 << "\n";

        float totalAngVel = speed2 - speed1;  
std::cout << "Total Ang Vel: " << totalAngVel << "\n";
std::cout << "Rel speed: " << relSpeed << "\n";

        // Decide if this collision is so low-impact that we should treat these
        //  two as in resting contact
        if (0) //relSpeed < 0.001f && fabs(totalAngVel) < 0.002f) // TODO
        {
std::cout << "Set as resting!\n";

          box1->SetResting(true);
          box2->SetResting(true);
          // TODO Zero rotational and linear vel
          box1->SetVel(Vec2f(0, 0));
          box1->SetAngVel(0);
          box2->SetVel(Vec2f(0, 0));
          box2->SetAngVel(0);
          return;
        }

        // Not in resting contact. 
        box1->SetResting(false);
        box2->SetResting(false);

        // As these two boxes are colliding, reduce their ang vel 
        // This is to try to improve stability
        box1->m_angVel *= 0.8f; // >0.8 and there is some bad instability
        box2->m_angVel *= 0.8f; 

        // Each box has proj of rel velocity onto contact normal added to vel.
        // This gives reflect vector if one box is immovable
        float dp = DotProduct(relvel, contactNormal); 
std::cout << "dp = " << dp << "\n";
        box1->m_vel -= frac1 * 2.0f * dp * contactNormal;
        box2->m_vel += frac2 * 2.0f * dp * contactNormal;
std::cout << "Box 1 NEW vel: " << box1->m_vel.x << ", " << box1->m_vel.y << "\n";
std::cout << "Box 2 NEW vel: " << box2->m_vel.x << ", " << box2->m_vel.y << "\n";

        // TODO TEMP TEST
        // Damp the response - this happens once per collision, so not multiplied by dt etc
        box1->m_vel *= 0.5f;
        box2->m_vel *= 0.5f;

        // Apply Torque
        // Mag of torque depends on how fast the contact point was moving
        //  relative to the face it penetrated.
        // i.e. Linear vel + rotational vel

        // Relative rotational vel: in 2D, rotation is only CW or CCW.

std::cout << "Contact point: " << c.m_pos.x << ", " << c.m_pos.y << "\n";
std::cout << "Box 1 pos: " << box1->m_pos.x << ", " << box1->m_pos.y << "\n"; 
std::cout << "Box 2 pos: " << box2->m_pos.x << ", " << box2->m_pos.y << "\n"; 

        float relSpeedMult = 0.1f; // 0.08f; // was ok before fixig dt
        float angVelMult = 0.02f; // 0.01f; 
        float mag = relSpeed * relSpeedMult + totalAngVel * angVelMult; 
std::cout << "Mag: " << mag << "\n";

        box1->AddTorque(-mag * contactNormal, c.m_pos);
        box2->AddTorque( mag * contactNormal, c.m_pos);

        box1->m_torqueArrow[0] = c.m_pos;
        box1->m_torqueArrow[1] = c.m_pos - mag * contactNormal;

//        box2->m_torqueArrow[0] = c.m_pos;
//        box2->m_torqueArrow[1] = c.m_pos + mag * contactNormal;

//PAUSE
std::cout << "\n";

        // This seems useless, as we are explicitly changing vel above, it doesn't
        //  make any difference -- except that it seems to have an effect on how
        //  things settle down.
        //box1->AddForce(-mag * contactNormal);
        //box2->AddForce( mag * contactNormal);
      }
}
static bool regRBBox2Collision = TheRBManager::Instance()->AddRBFunc(
  RBBox2::TYPENAME, RBBox2::TYPENAME, (RBManager::RBFunc)RBBox2Collision);

}
