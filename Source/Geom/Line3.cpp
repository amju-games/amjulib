#include "Line3.h"

namespace Amju
{
float LineSeg::SqDist(const Vec3f& q)
{
  Vec3f m(p1 - p0);
  float mDotM = DotProduct(m, m);
  const float EPSILON = 0.000001f;
  if (fabs(mDotM) < EPSILON)
  {
    return (q - p0).SqLen();
  }
  float t = DotProduct(m, (q - p0)) / mDotM;
  if (t < 0)
  {
    return (q - p0).SqLen();
  }
  else if (t > 1.0f)
  {
    return (q - p1).SqLen();
  }
  else
  {
    Vec3f p = p0 + t * m;
    return (q - p).SqLen();
  }
}
}

