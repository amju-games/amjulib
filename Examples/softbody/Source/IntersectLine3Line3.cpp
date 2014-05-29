#include "IntersectLine3Line3.h"

namespace Amju
{
/*
   Calculate the line segment PaPb that is the shortest route between
   two lines P1P2 and P3P4. Calculate also the values of mua and mub where
      Pa = P1 + mua (P2 - P1)
      Pb = P3 + mub (P4 - P3)
   Return FALSE if no solution exists.
*/
// From: http://paulbourke.net/geometry/pointlineplane/lineline.c
// Adapted to use Line Seg type.

bool Intersects(const LineSeg& ls1, const LineSeg& ls2, 
  LineSeg* shortest, float* mua, float* mub)
{
   const float EPS = 0.0001f;

   Vec3f p13,p43,p21;
   float d1343,d4321,d1321,d4343,d2121;
   float numer,denom;

   const Vec3f& p1 = ls1.p0;
   const Vec3f& p2 = ls1.p1;
   const Vec3f& p3 = ls2.p0;
   const Vec3f& p4 = ls2.p1;

   p13 = p1 - p3;
   //p13.x = p1.x - p3.x;
   //p13.y = p1.y - p3.y;
   //p13.z = p1.z - p3.z;

   p43 = p4 - p3;
   //p43.x = p4.x - p3.x;
   //p43.y = p4.y - p3.y;
   //p43.z = p4.z - p3.z;

   if (fabs(p43.x) < EPS && fabs(p43.y) < EPS && fabs(p43.z) < EPS)
      return(false);

   p21 = p2 - p1;
   //p21.x = p2.x - p1.x;
   //p21.y = p2.y - p1.y;
   //p21.z = p2.z - p1.z;

   if (fabs(p21.x) < EPS && fabs(p21.y) < EPS && fabs(p21.z) < EPS)
      return(false);

   d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
   d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
   d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
   d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
   d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

   denom = d2121 * d4343 - d4321 * d4321;
   if (fabs(denom) < EPS)
      return(false);
   numer = d1343 * d4321 - d1321 * d4343;

   *mua = numer / denom;
   *mub = (d1343 + d4321 * (*mua)) / d4343;

   Vec3f pa, pb;
   pa.x = p1.x + *mua * p21.x;
   pa.y = p1.y + *mua * p21.y;
   pa.z = p1.z + *mua * p21.z;
   pb.x = p3.x + *mub * p43.x;
   pb.y = p3.y + *mub * p43.y;
   pb.z = p3.z + *mub * p43.z;

   *shortest = LineSeg(pa, pb);

   return(true);
}

}

