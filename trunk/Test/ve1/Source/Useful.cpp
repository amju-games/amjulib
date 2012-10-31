#include <sstream>
#include <StringUtils.h>
#include "Useful.h"

namespace Amju
{
std::ostream& operator<<(std::ostream& os, const Vec3f& v)
{
  return os << "x: " << v.x << " y: " << v.y << " z: " << v.z;
}

std::string ToString(const Vec3f& v)
{
  std::stringstream ss;
  ss << v;
  return ss.str();

//  return std::string("x: ") + ToString(v.x) + std::string(" y: ") + ToString(v.y) + std::string(" z: ") + ToString(v.z);
}

Vec3f ToVec3(const std::string& s)
{
  Strings strs = Split(s, ',');
  Assert(strs.size() == 3);
  return Vec3f(ToFloat(strs[0]), ToFloat(strs[1]), ToFloat(strs[2]));
}

std::ostream& operator<<(std::ostream& os, const GameObject& go)
{
  return os << "Object " << go.GetId() << " (" << go.GetTypeName() << ")";
}

std::string ToString(const GameObject& go)
{
  std::stringstream ss;
  ss << go;
  return ss.str();
}

}

