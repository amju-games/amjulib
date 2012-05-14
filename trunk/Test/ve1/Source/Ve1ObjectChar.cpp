#include <Timer.h>
#include <DegRad.h>
#include "Ve1ObjectChar.h"
#include "Ve1Character.h"

namespace Amju
{
Ve1ObjectChar::Ve1ObjectChar()
{
  m_dir = 0;
  m_dirCurrent = m_dir;
  m_isMoving = false;
  m_inNewLocation = false;
}

const std::string& Ve1ObjectChar::GetName() const
{
  return m_name;
}

void Ve1ObjectChar::SetName(const std::string& name)
{
  m_name = name;
}

void Ve1ObjectChar::MoveTo(const Vec3f& newpos)
{
  m_newPos = newpos;
  m_isMoving = true;

//std::cout << "Player: got new pos to move to: " << newpos << ", current pos is " << GetPos() << "\n";

  Vec3f dir = GetPos() - newpos;

  // Why TF was this commented out??!?!?!
  if (dir.SqLen() < 1.0f) // TODO CONFIG
  {
    SetVel(Vec3f(0, 0, 0));
// TODO    SetArrowVis(false);
  }
  else
  {
    dir.Normalise();
    static const float SPEED = 50.0f; // TODO CONFIG
    SetVel(-dir * SPEED);

    // Work out direction to face
    SetDir(RadToDeg(atan2((double)m_vel.x, (double)m_vel.z)));
  }
}

void Ve1ObjectChar::SetDir(float degs)
{
  m_dir = degs;
}

void Ve1ObjectChar::TurnToFaceDir()
{
  Matrix mat;
  mat.RotateY(DegToRad(m_dirCurrent));
  mat.TranslateKeepRotation(m_pos);
  m_sceneNode->SetLocalTransform(mat);

  static const float ROT_SPEED = 10.0f; // TODO CONFIG
  float dt = TheTimer::Instance()->GetDt();
  float angleDiff = m_dir - m_dirCurrent;

  // Rotate to face m_dir, taking the shortest route (CW or CCW)
  if (fabs(angleDiff) < 10.0f)
  {
    m_dirCurrent = m_dir;
  }
  else
  {
    if (angleDiff < -180.0f)
    {
      m_dir += 360.0f;
    }
    else if (angleDiff > 180.0f)
    {
      m_dir -= 360.0f;
    }
    angleDiff = m_dir - m_dirCurrent;

    if (m_dirCurrent > m_dir)
    {
      m_dirCurrent -= ROT_SPEED * dt * fabs(angleDiff);
    }
    else if (m_dirCurrent < m_dir)
    {
      m_dirCurrent += ROT_SPEED * dt * fabs(angleDiff);
    }
  }
}

AABB* Ve1ObjectChar::GetAABB()
{
  return m_sceneNode->GetAABB();
}


}

