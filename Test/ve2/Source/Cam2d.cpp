#include <AmjuFirst.h>
#include "Cam2d.h"
#include "ROConfig.h"
#include "Room.h"
#include <AmjuFinal.h>

namespace Amju
{
Cam2d::Cam2d()
{
  m_target = 0;
}

void Cam2d::Update()
{
  Vec3f pos;
  if (m_target)
  {
    pos = m_target->GetPos();
  }

  Room* room = GetRoom();
  if (room)
  {
    Vec2f size = room->GetSize() * 0.5f;

std::cout << "Cam: pos.x: " << pos.x << " size.x: " << size.x << "\n";

    float MAX_X = 500.0f; 
    if (pos.x + size.x > MAX_X)
    {
      pos.x = MAX_X - size.x;
    }
    else if (pos.x - size.x < -MAX_X)
    {
      pos.x = size.x - MAX_X;
    }

    float MAX_Y = 400.0f;
    if (pos.z + size.y > MAX_Y)
    {  
      pos.z = MAX_Y - size.y;
    }
    else if (pos.z - size.y < -MAX_Y)
    {
      pos.z = size.y - MAX_Y;
    } 
  }

  SetLookAtPos(pos);
  static const float HEIGHT = ROConfig()->GetFloat("cam-z", 300.0f);
  SetEyePos(pos + Vec3f(0, HEIGHT, 2.0f)); // TODO TEMP TEST
}

void Cam2d::Reset()
{
}

}
