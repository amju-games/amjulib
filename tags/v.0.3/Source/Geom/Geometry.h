/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Geometry.h,v $
Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#if !defined(GEOM_H_INCLUDED)
#define GEOM_H_INCLUDED

#include "Vec3.h"

namespace Amju
{
void MakeNormal(const float v1[3], 
                       const float v2[3], 
                       const float v3[3], 
                       float result[3]);

void MakeNormal(const Vec3f& v1,
                const Vec3f& v2,
                const Vec3f& v3,
                Vec3f* pResult);

void CrossProduct(float v1[3], float v2[3], float result[3]);

void Normalize(float r[3]);

// Get the angle in the x-z plane between the first vertex and the
// second.
float GetXZAngle(const Vec3f& v1, const Vec3f& v2);
}
#endif
