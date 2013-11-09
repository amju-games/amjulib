#include <AmjuFirst.h>
#include "SceneNodeCamera.h"
#include "AmjuGL.h"
#include <AmjuFinal.h>

namespace Amju
{
SceneNodeCamera::SceneNodeCamera()
{
  m_up = Vec3f(0, 1, 0);
}

const Vec3f& SceneNodeCamera::GetEyePos() const
{
  return m_eye;
}

const Vec3f& SceneNodeCamera::GetLookAtPos() const
{
  return m_lookat;
}

void SceneNodeCamera::SetEyePos(const Vec3f& eye)
{
  m_eye = eye;
}

void SceneNodeCamera::SetLookAtPos(const Vec3f& lookat)
{
  m_lookat = lookat;
}

void SceneNodeCamera::SetUpVec(const Vec3f& up)
{
  m_up = up;
}

void SceneNodeCamera::Draw()
{
  // Don't set Identity - camera can be attached to something; multiple cameras..?
  AmjuGL::LookAt(
    m_eye.x, m_eye.y, m_eye.z,  // eye
    m_lookat.x, m_lookat.y, m_lookat.z,  // target
    m_up.x, m_up.y, m_up.z); // up
}

Matrix SceneNodeCamera::GetMatrix() const
{
  Vec3f forward = m_lookat - m_eye;
  forward.Normalise();
  Vec3f right = CrossProduct(forward, m_up);
  right.Normalise();
  Vec3f up = CrossProduct(right, forward);
  Matrix m;
  m.SetIdentity();
  m[0] = right.x;
  m[1] = right.y;
  m[2] = right.z;
  m[4] = up.x;
  m[5] = up.y;
  m[6] = up.z;
  m[8] = forward.x;
  m[9] = forward.y;
  m[10] = forward.z;
  return m;
}

}

