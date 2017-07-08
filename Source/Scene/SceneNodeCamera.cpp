#include <AmjuFirst.h>
#include <AmjuGL.h>
#include "File.h"
#include "LoadVec3.h"
#include "SceneNodeCamera.h"
#include "StringUtils.h"
#include <AmjuFinal.h>

namespace Amju
{
const char* SceneNodeCameraPersp::NAME = "camera-persp";

const char* SceneNodeCameraOrtho::NAME = "camera-ortho";

SceneNodeCamera::SceneNodeCamera()
{
  m_up = Vec3f(0, 1, 0);
}

void SceneNodeCamera::SetFromCamera(const Camera& c)
{
  SetEyePos(c.m_pos);
  SetLookAtPos(c.m_pos + c.m_dir);
  SetUpVec(c.m_up);
}

bool SceneNodeCamera::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected scene node name");
    return false;
  }

  if (!LoadVec3(f, &m_eye))
  {
    f->ReportError("Failed to load eye pos for camera node");
    return false;
  }

  if (!LoadVec3(f, &m_lookat))
  {
    f->ReportError("Failed to load look at (target) pos for camera node");
    return false;
  }

  if (!LoadVec3(f, &m_up))
  {
    f->ReportError("Failed to load up vector for camera node");
    return false;
  }
  return true;
}

bool SceneNodeCameraPersp::Load(File* f)
{
  if (!SceneNodeCamera::Load(f))
  {
    return false;
  }

  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected projection params (fov, aspect, near, far) for camera node");
    return false;
  }
  Strings strs = Split(s, ',');
  if (strs.size() != 4)
  {
    f->ReportError("Expected projection params (fov, aspect, near, far) for camera node");
    return false;
  }
  for (int i = 0; i < 4; i++)
  {
    m_perspective[i] = ToFloat(strs[i]);
  }

  if (!LoadChildren(f))
  {
    return false;
  }

  return true;
}

bool SceneNodeCameraOrtho::Load(File* f)
{
  if (!SceneNodeCamera::Load(f))
  {
    return false;
  }

  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected ortho params (left, right, top, bottom, near, far) for camera node");
    return false;
  }
  Strings strs = Split(s, ',');
  if (strs.size() != 6)
  {
    f->ReportError("Expected ortho params(left, right, top, bottom, near, far) for camera node");
    return false;
  }
  for (int i = 0; i < 6; i++)
  {
    m_ortho[i] = ToFloat(strs[i]);
  }

  if (!LoadChildren(f))
  {
    return false;
  }

  return true;
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

void SceneNodeCameraPersp::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetPerspectiveProjection(
    m_perspective[0], m_perspective[1], m_perspective[2], m_perspective[3]);

  SceneNodeCamera::Draw();
}

void SceneNodeCameraOrtho::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetOrthoProjection(
    m_ortho[0], m_ortho[1], m_ortho[2], m_ortho[3], m_ortho[4], m_ortho[5]);

  SceneNodeCamera::Draw();
}

void SceneNodeCamera::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
  // TODO Not sure this comment makes sense
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

