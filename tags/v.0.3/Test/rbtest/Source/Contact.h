#ifndef AMJU_CONTACT_H
#define AMJU_CONTACT_H

#include <Vec3.h>
#include <Vec2.h>

namespace Amju
{
struct Contact2
{
  Contact2() { m_time = 0; m_penetrationDepth = 0; } 

  float m_time;
  Vec2f m_contactNormal;
  Vec2f m_pos;
  float m_penetrationDepth;
};

struct Contact3
{
  Contact3() { m_time = 0; m_penetrationDepth = 0; } 

  float m_time;
  Vec3f m_contactNormal;
  Vec3f m_pos;
  float m_penetrationDepth; 
};
}

#endif

