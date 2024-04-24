#ifndef LOAD_VEC2_H
#define LOAD_VEC2_H

#include <sstream>
#include <string>
#include "Vec2.h"

namespace Amju
{
class File;

bool LoadVec2(File* f, Vec2f* v);
bool LoadVec2(File* f, Vec2i* v);

bool ToVec2(const std::string& s, Vec2f* v);
bool ToVec2(const std::string& s, Vec2i* v);

template<class T>
std::string ToString(const Vec2<T>& v)
{
  std::stringstream ss;
  ss << v.x << "," << v.y;
  return ss.str();
}
}

#endif

