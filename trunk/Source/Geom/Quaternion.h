/*
Amju Games source code (c) Copyright Jason Colman 2005
*/

#ifndef AMJU_QUATERNION_H_INCLUDED
#define AMJU_QUATERNION_H_INCLUDED

#include <string>
#include "Vec3.h"

namespace Amju
{
class Matrix;
class File;

class Quaternion  
{
public:
  Quaternion();
  Quaternion(float w, float x, float y, float z);
  Quaternion(float w, const Vec3f&);

  void SetIdentity();

  Quaternion operator*(const Quaternion& q) const;

  void CreateMatrix(Matrix* pMatrix) const;
  void CreateMatrix(float* pMatrix) const;

  void CreateFromAxisAngle(float x, float y, float z, float degrees);
  void CreateFromMatrix(const Matrix& m);

  static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float between);

  float SquaredLength() const;

  void Normalize();

  bool Load(File*);

#ifdef SCENE_EDITOR
  bool Save(File*);
#endif

  friend std::string ToString(const Quaternion& q);

  // Return result of rotating vector by this quaternion
  Vec3f RotateVec(const Vec3f& v) const;

  // Return conjugate of this quaternion
  Quaternion Conjugate() const;

private:
  float m_w;
  float m_z;
  float m_y;
  float m_x;
};

}

#endif 
