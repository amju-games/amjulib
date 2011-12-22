#include "Ve1Object.h"

namespace Amju
{
void Ve1Object::MoveTo(const Vec3f& pos)
{
  SetPos(pos);
}

void Ve1Object::Set(const std::string& key, const std::string& val)
{
  if (key == "name")
  {
    m_name = val;
  }  
  // ...etc...
}

}

