#ifndef LOAD_VEC3_H
#define LOAD_VEC3_H

#include "Vec3.h"

namespace Amju
{
  class File;
  bool LoadVec3(File* f, Vec3f* v);
  bool SaveVec3(File* f, const Vec3f& v);
}

#endif

