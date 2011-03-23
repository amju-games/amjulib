#ifndef AMJU_LINE3_INCLUDED
#define AMJU_LINE3_INCLUDED

#include "Vec3.h"

namespace Amju
{
struct LineSeg
{
  LineSeg() {}
  LineSeg(const Vec3f& p0_, const Vec3f& p1_) : p0(p0_), p1(p1_) {}
  void Draw();
  // Point to LineSeg
  float SqDist(const Vec3f& p);

  Vec3f p0, p1; // endpoints, not start+dir
};
}

#endif