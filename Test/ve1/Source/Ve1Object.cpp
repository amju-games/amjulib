#include "Ve1Object.h"

namespace Amju
{
Ve1Object::Ve1Object() : m_location(-1)
{
}

int Ve1Object::GetLocation() const
{
  return m_location;
}

void Ve1Object::SetLocation(int loc)
{
  m_location = loc;
}

void Ve1Object::MoveTo(const Vec3f& pos, int location)
{
  // TODO Remove from game & scene graph etc if not in same location as local player

  SetPos(pos);
}

void Ve1Object::Set(const std::string& key, const std::string& val)
{
  // TODO
}

}

