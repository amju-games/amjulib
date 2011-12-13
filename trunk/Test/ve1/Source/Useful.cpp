#include "Useful.h"
#include <StringUtils.h>

namespace Amju
{
std::ostream& operator<<(std::ostream& os, const Vec3f& v)
{
  return os << "x: " << v.x << " y: " << v.y << " z: " << v.z;
}

std::string ToString(const Vec3f& v)
{
  return std::string("x: ") + ToString(v.x) + std::string(" y: ") + ToString(v.y) + std::string(" z: ") + ToString(v.z);
}
}

