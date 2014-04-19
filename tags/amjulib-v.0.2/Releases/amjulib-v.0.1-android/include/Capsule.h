#ifndef CAPSULE_H
#define CAPSULE_H

#include "Line3.h"

namespace Amju
{
struct Capsule
{
  Capsule(const LineSeg& seg, float radius) : m_lineseg(seg), m_radius(radius) { }

  LineSeg m_lineseg;
  float m_radius;
};
}

#endif
