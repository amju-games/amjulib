#include "RBIntersect.h"
#include "RBSphere.h"
#include "AmjuAssert.h"

namespace Amju
{
bool Intersects(const RBSphere& s1, const RBSphere s2, float* timeFirstContact)
{
  // Calc rel vel, treat one s1 as stationary
  Vec3f m = s2.GetVel() - s1.GetVel(); // rel vel; dir of capsule ray
  Vec3f s = s1.GetPos() - s2.GetPos();
  float sumRadii = s1.GetRadius() + s2.GetRadius();
  float sqSumRadii = sumRadii * sumRadii;

  /*
  // Intersection between static sphere and moving sphere ?
  // Get closest point on line from s2 pos in direction of rel vel
  
  // m is direction, i.e. relVel
  float mDotM = DotProduct(m, m);
  float t = 0;
  if (mDotM > 0.00001f) // handle low speed/stationary s2
  {
    t = DotProduct(m, (s1.GetPos() - s2.GetPos())) / mDotM;
  }
  if (t < 0)
  {
    // Collision in past, ignore
    // TODO check this is safe
    return false;
  }

  Vec3f p = s2.GetPos() + t * m; // point along line of rel vel where spheres are closest
  float sqDist = (s1.GetPos() - p).SqLen();
  if (sqDist > sqSumRadii)
  {
    // Shortest dist greater than sum of radii, so no collision
    return false;
  }

  // t is time when spheres are closest, not time of first contact.
  */

  // Calc t for first contact
  // Let s = p0 - q (ie diff between sphere positions); m = p1 - p0 (dir of line)
  // Spheres are just touching when |s + tm| = r, or squaredLen(s + tm) = r*r.
  // (s + tm) dot (s + tm) - r * r = 0
  // Multiplying out dot products,
  // t^2 (m . m) + t (2 * m . s) + (s . s - r^2) = 0
  // This is a quadratic eq with possibly 2 roots (solutions)
  // TODO Quadratic formula can be slightly simpler when b is even, which it is here
  float a = DotProduct(m, m);
  float b = DotProduct(s, m) * 2.0f;
  float c = DotProduct(s, s) - sqSumRadii;
  // From numerical recipes in C, section 5.6
  float bSqMinus4ac = b * b - 4.0f * a * c;
  if (bSqMinus4ac < 0)
  {
    return false;
  }
  float q = b > 0 ? -0.5f * (b + sqrt(bSqMinus4ac)) : -0.5f * (b - sqrt(bSqMinus4ac));
  // The two roots are q / a and c / q - we want the smaller (i.e. first time, not second)
  *timeFirstContact  = std::min(q / a, c / q);

  return true;
}
}
