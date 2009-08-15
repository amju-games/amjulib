/*
Amju Games source code (c) Copyright Jason Colman 2004-2009
*/

#include "AmjuFirst.h"
#ifdef MSVC
#define _USE_MATH_DEFINES
#endif // MSVC
#include <math.h>
#include "Geometry.h"
#include "AmjuFinal.h"

namespace Amju
{
float GetXZAngle(const Vec3f& v1, const Vec3f& v2)
{
  AMJU_CALL_STACK;

  float dx = v2.x - v1.x;
  float dz = v2.z - v1.z;
  float r = atan2(dx, dz); // TODO do we need to check if z = 0 ?
  float rDegs = r / (float)M_PI * 180.0f; // convert to degs
  rDegs += 180.0f; // need this to fix z direction 
  return rDegs;
}

void Normalize(float r[3])
{
  AMJU_CALL_STACK;

  const float len = (float) sqrt(r[0]*r[0]+r[1]*r[1]+r[2]*r[2]);
  if (len != 0.0f)
  {
    r[0]/=len;
    r[1]/=len;
    r[2]/=len;
  }
}

void CrossProduct(float v1[3], float v2[3], float result[3])
{
  AMJU_CALL_STACK;

  result[0] = v1[1]*v2[2]-v2[1]*v1[2];
  result[1] = v2[0]*v1[2]-v1[0]*v2[2];
  result[2] = v1[0]*v2[1]-v2[0]*v1[1];
}

void MakeNormal(const Vec3f& v0,
                const Vec3f& v1,
                const Vec3f& v2,
                Vec3f* pResult)
{
  AMJU_CALL_STACK;

  Vec3f cp = CrossProduct(v1 - v0, v2 - v0);
  cp.Normalise();
  *pResult = cp;
}

void MakeNormal(
  const float v1[3], 
  const float v2[3], 
  const float v3[3], 
  float r[3])
{
  AMJU_CALL_STACK;

    float t1[3];
    float t2[3];

    t1[0] = v1[0]-v2[0];
    t1[1] = v1[1]-v2[1];
    t1[2] = v1[2]-v2[2];

    t2[0] = v2[0]-v3[0];
    t2[1] = v2[1]-v3[1];
    t2[2] = v2[2]-v3[2];

    CrossProduct(t1, t2, r);
    Normalize(r);
}
} 
