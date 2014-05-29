#pragma once

#include <Line3.h>

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
  LineSeg* shortest, float* mua, float* mub);
}

