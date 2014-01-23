/*
Amju Games source code (c) Copyright Jason Colman 2000-2009
*/

#ifndef AMJU_AABB_H
#define AMJU_AABB_H

#include "Vec3.h"

namespace Amju
{
class Matrix;

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
  void Transform(const Matrix& m);

  bool Intersects(const AABB&) const;
  bool Intersects(const Vec3f& point) const;

  float GetXSize() const;
  float GetYSize() const;
  float GetZSize() const;

  Vec3f CalcCentre() const;

  // Get min and max for axis 0..2
  float GetMin(int axis) const;
  float GetMax(int axis) const;

  void SetMin(int axis, float min);
  void SetMax(int axis, float max);

  // Enlarge this box as required, so that it encloses its original
  //  volume and the new box.
  void Union(const AABB&);

  // Calc intersection between two boxes
  AABB Intersection(const AABB&) const;

private:
  float m_xmin, m_xmax, m_ymin, m_ymax, m_zmin, m_zmax;
};
}

#endif
