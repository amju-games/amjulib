#include "Terrain.h"
#include <iostream>
#include <DrawOBB3.h>
#include <AmjuGL.h>

namespace Amju
{
Terrain* GetTerrain()
{
  // TODO
  static Terrain* t = new Terrain;
  return t;
}

void Terrain::Draw()
{
  static OBB3 obb;
  obb.SetExtents(Vec3f(100, 1.0, 100));
  PushColour();
  AmjuGL::SetColour(Colour(1, 0, 0, 1));
  DrawSolidOBB3(obb);
  PopColour();
}

Vec3f Terrain::GetMousePos(const LineSeg& seg)
{
  // Find intersection of line seg and terrain triangles - Octree would make this efficient
  // TODO
  // Assume ground plane at y=0. Find (x, z) at this y value.
  // Point p = P0 + t(P1 - P0)    =>   t = (p - P0) / (P1 - P0)
  //  p.y = 0, so t = - P0.y / (P1.y - P0.y)
  float m = (seg.p1.y - seg.p0.y); 
  if (m == 0)
  {
    std::cout << "Can't get mouse pos, line parallel to ground ?\n";
    return Vec3f();
  }
  else
  {
    float t = - seg.p0.y / m;
    if (t < 0 || t > 1)
    {
std::cout << "Unexpected value for t! " << t << "\n";
      return Vec3f();      
    }
std::cout << "t: " << t << "\n";
    return seg.GetPoint(t);
  }
}
}

