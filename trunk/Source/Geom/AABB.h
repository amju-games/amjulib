/*
Amju Games source code (c) Copyright Jason Colman 2000-2009
*/

#ifndef AMJU_AABB_H
#define AMJU_AABB_H

#include "Vec3.h"

namespace Amju
{
class AABB
{
public:
  AABB();
  AABB(
    float xmin, 
    float xmax, 
    float ymin, 
    float ymax, 
    float zmin, 
    float zmax);

  void Set(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
  
  // Set values which are lower than the mins or greater than maxes.
  // I.e. enlarge the box to contain (x, y, z) if necessary.
  void SetIf(float x, float y, float z);

  void Translate(const Vec3f&);

  bool Intersects(const AABB&) const;

  void Draw() const;

  float GetXSize() const;
  float GetYSize() const;
  float GetZSize() const;

private:
  float m_xmin, m_xmax, m_ymin, m_ymax, m_zmin, m_zmax;
};
}

#endif
