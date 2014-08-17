#include <AmjuGL.h>
#include "Camera.h"

namespace Amju
{
Camera::Camera()
{
  m_fov = 45.0f; // degrees
  m_near = 1.0f;
  m_far = 1000.0f;
  m_aspect = 1.0f;
  eyeSep = 0;

  m_up = Vec3f(0, 1, 0);
  m_dir = Vec3f(0, 0, 1);
}

void Camera::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetPerspectiveProjection(m_fov, m_aspect, m_near, m_far);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
  Vec3f v = m_pos + m_dir;
  AmjuGL::LookAt(m_pos.x, m_pos.y, m_pos.z, v.x, v.y, v.z, m_up.x, m_up.y, m_up.z);
}

}

