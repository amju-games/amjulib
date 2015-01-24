#include <AmjuGL.h>
#include "Camera.h"

namespace Amju
{
Camera::Camera()
{
  m_fovy = 45.0f; // degrees
  m_neardist = 1.0f;
  m_fardist = 1000.0f;
  m_aspect = 1.0f;
  m_eyeSep = 0;
  m_fo = 75.0f; // from  http://paulbourke.net/stereographics/stereorender/ - not clear what this is for

  m_up = Vec3f(0, 1, 0);
  m_dir = Vec3f(0, 0, 1);

  m_vpXOffset = 0.08f;
  m_vpYOffset = 0.04f;
}

void Camera::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetPerspectiveProjection(m_fovy, m_aspect, m_neardist, m_fardist);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
  Vec3f v = m_pos + m_dir;
  AmjuGL::LookAt(m_pos.x, m_pos.y, m_pos.z, v.x, v.y, v.z, m_up.x, m_up.y, m_up.z);
}

}

