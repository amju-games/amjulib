#ifndef AMJU_CONTACT_H
#define AMJU_CONTACT_H

#include <Vec3.h>

namespace Amju
{
struct Contact
{
  float m_time;
  Vec3f m_contactNormal;
  Vec3f m_pos;
};
}

#endif

