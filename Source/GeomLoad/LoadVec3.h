#ifndef LOAD_VEC3_H
#define LOAD_VEC3_H

#include <sstream>
#include <string>
#include <StringUtils.h>
#include "Vec3.h"

namespace Amju
{
class File;
bool LoadVec3(File* f, Vec3f* v);

// Load a Vec3, with optional z coord. If only (x, y) coords are
//  specified, z defaults to the given value.
bool LoadVec3OptionalZ(File* f, Vec3f* v, float defaultZ = 0);

bool SaveVec3(File* f, const Vec3f& v);

template <typename T>
bool ToVec3(const std::string& s, Vec3<T>* v)
{
  auto vec = SplitIntoVec<T>(s, ',');
  if (vec.size() != 3)
  {
    return false;
  }
  *v = Vec3<T>(vec[0], vec[1], vec[2]);
  return true;
}

template<class T>
std::string ToString(const Vec3<T>& v)
{
  std::stringstream ss; 
  ss << v.x << "," << v.y << "," << v.z;
  return ss.str();
}
}

#endif

