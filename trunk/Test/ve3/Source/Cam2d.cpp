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

    float MAX_X = 250.0f;  // visible to L/R of player

    if (size.x - pos.x < MAX_X)
    {
      pos.x = size.x - MAX_X;
    }
    else if (pos.x + size.x < MAX_X)
    {
      pos.x = -size.x + MAX_X;
    }
  
    float MAX_Y = 150.0f; 
    if (size.y - pos.z < MAX_Y)
    {
      pos.z = size.y - MAX_Y;
    }
    else if (pos.z + size.y < MAX_Y)
    {
      pos.z = -size.y + MAX_Y;
    }
  }

  SetLookAtPos(pos);

  static const float HEIGHT = ROConfig()->GetFloat("cam-z", 300.0f);
  SetEyePos(pos + Vec3f(0, HEIGHT, 0)); // TODO TEMP TEST
  SetUpVec(Vec3f(0, 0, -1));
}

void Cam2d::Reset()
{
}

}
