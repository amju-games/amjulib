#include "Cam2d.h"
#include "ROConfig.h"

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

  SetLookAtPos(pos);
  static const float HEIGHT = ROConfig()->GetFloat("cam-z", 300.0f);
  SetEyePos(pos + Vec3f(0, HEIGHT, 2.0f)); // TODO TEMP TEST
}

void Cam2d::Reset()
{
}

}
