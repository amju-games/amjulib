#include <AmjuFirst.h>
#include <iostream>
#include "ClipLineSegBox.h"
#include <AmjuFinal.h>

// Valid points on the line seg are m_start + t * m_direction for
//  t in [0..1] for x, y and z.
// As we check the line against the planes of the box, we reduce 
//  the valid range of t. It starts at [t0.. t1] where t0 starts at 0,
//  and t1 starts at 1.
//
// First we check for intersection with the min. x plane of the box.
// i.e. we solve for m_start.x + t * m_direction.x == box.minx.
// so t = (box.minx - m_start.x) / m_direction.x.
// We can call m_direction.x the denominator, and 
//  (box.minx - m_start.x) the numerator.
//
// If m_direction.x is 0, we are parallel to the box plane. In this
//  case we are either inside or outside the plane: inside if
//  m_start.x >= box.minx, in other words if the numerator is pos.
// If m_direction.x is positive (the line is moving to the right): 
//  We check if t > t1, t < t0, or to < t < t1.
//  If t > t1 the line seg is to the left of the plane - return false.
//  If t < t0 the line seg is to the right of the plane, so ret true.
//  Otherwise the line is clipped. t0 now becomes t.
// If m_direction.x is negative, it's the same idea but in the opposite
//  direction.
//  If t < t0, the line is completely to the left of the plane.
//  If t > t1, the line is completely on the right.
//  Otherwise the line is clipped; the new value for t1 is t.

namespace Amju
{
bool ClipAxis(
    float min, float max, float start, float direction, 
    float& t0, float& t1)
{
    if (direction > 0)
    {
        // Moving right
        float t = (min - start) / direction;
        if (t > t1)
        {
            return false; // completely clipped
        }
        if (t > t0)
        {
            t0 = t;
        }
    }
    else if (direction < 0)
    {
        // Moving left
        float t = (min - start) / direction;
        if (t < t0)
        {
            return false;
        }
        if (t < t1)
        {
            t1 = t;
        }
    }
    else if (start < min)
    {
        return false;
    }


    // Check against box max X
    if (direction > 0)
    {
        // Moving right
        float t = (max - start) / direction;
        if (t < t0)
        {
            return false;
        }
        if (t < t1)
        {
            t1 = t;
        }
    }
    else if (direction < 0)
    {
        float t = (max - start) / direction;
        if (t > t1)
        {
            return false;
        }
        if (t > t0)
        {
            t0 = t;
        }
    }
    else if (start > max)
    {
        return false;
    }

    return true;
}

bool Clip(const LineSeg& ls, const AABB& box, LineSeg* clipped)
{
    float t0 = 0;
    float t1 = 1.0f;

    Vec3f dir = ls.p1 - ls.p0;

    if (!ClipAxis(box.GetMin(0), box.GetMax(0), ls.p0.x, dir.x, t0, t1))
    {
        return false;
    }

    if (!ClipAxis(box.GetMin(1), box.GetMax(1), ls.p0.y, dir.y, t0, t1))
    {
        return false;
    }

    if (!ClipAxis(box.GetMin(2), box.GetMax(2), ls.p0.z, dir.z, t0, t1))
    {
        return false;
    }

    if (clipped)
    {
        *clipped = LineSeg(
            ls.p0 + dir * t0,
            ls.p0 + dir * t1);
    }

    return true;
}

// Clip line seg against oriented box. Same idea as for axis aligned box, but we clip against the 
//  box planes.
bool Clip(const LineSeg& lineSeg, const OBB3& box, LineSeg* clipped)
{
  Plane planes[6];
  box.GetPlanes(planes);

  // Get value of t in line equation for intersection of all 6 planes. 
/*
  Lines seg: p = p0 + t (p1 - p0)
  Point on Plane; p . N - D = 0
  Intersection, solve for t. 
  [ p0 + t (p1 - P0) ] . N - D = 0
  p0 . N + [t (p1 - p0)] . N - D = 0
  p0 . N + t (p1 - p0) . N - D = 0
  t = (D - p0 . N) / [(p1 - p0) . N]
  => no lineseg-plane intersection if (p1 - p0) . N == 0. We go on to next slab.
  For each slab, get two t values. t1 is set to max if less than t1, t0 is set to min if more than t0..?
*/

  // Planes are in three groups of 2. Each pair of planes are parallel. The Normals are opposite.
  Vec3f dir(lineSeg.p1 - lineSeg.p0);

  const float VERY_SMALL = 0.001f;
  float t1 = 1.0f;
  float t0 = 0;

//std::cout << "CLIPPING....\n";

  for (int i = 0; i < 6; i+= 2)
  {
    float dp = DotProduct(planes[i].Normal(), dir);
    if (fabs(dp) < VERY_SMALL)
    {
      // Parallel - no intersection
//std::cout << "Clip: line seg is parallel to plane " << i << "\n";
      // If in front of either plane, line seg does not intersect box
      if (planes[i].Dist(lineSeg.p0) > 0  || planes[i + 1].Dist(lineSeg.p0) > 0)
      {
//std::cout << "Clip: line seg is parallel and in front of plane, so no intersection\n";
        return false;
      }
      continue; //return false;
    }
//std::cout << "Plane " << i << " dp: " << dp << "\n";  
//std::cout << "Plane " << i << " D: " << planes[i].D() << "\n";
//std::cout << "Plane " << i+1 << " D: " << planes[i+1].D() << "\n";

    float minmax[2] = 
    {
      (planes[i].D() - DotProduct(lineSeg.p0, planes[i].Normal())) /  dp,
      (planes[i + 1].D() - DotProduct(lineSeg.p0, planes[i + 1].Normal())) / -dp,
    }; 

    if (minmax[0] > minmax[1]) 
    {
      std::swap(minmax[0], minmax[1]);
    }
//std::cout << "Clip: plane " << i << " min: " << minmax[0] << " max: " << minmax[1] << "\n";

    if (minmax[0] > t0)
    {
      t0 = minmax[0];
    }

    if (minmax[1] < t1)
    {
      t1 = minmax[1];
    }

    if (t0 > t1 || t1 - t0 < VERY_SMALL)
    {
//std::cout << "Clip: no intersection, t0 >= t1\n";
      return false;
    }
  }
  
  if (clipped)
  {
    *clipped = LineSeg(
        lineSeg.p0 + dir * t0,
        lineSeg.p0 + dir * t1);
  }

//std::cout << "Clip: t0: " << t0 << " t1: " << t1 << "\n";
//std::cout << "Clip: found intersection!\n";
  return true;
}

}
