#pragma once

#include <Vec3.h>

namespace Amju
{
class Camera
{
public:
  Camera();
  void SetPos(const Vec3f v) { m_pos = v; }
  void SetDir(const Vec3f v) { m_dir = v; }
  void SetUp(const Vec3f v) { m_up = v; }
  void SetNearFar(float n, float f) { m_near = n; m_far = f; }
  void SetAspect(float a) { m_aspect = a; }

  void Draw();

private:
  Vec3f m_pos;
  Vec3f m_dir;
  Vec3f m_up;
  float m_fov;
  float m_near;
  float m_far;
  float m_aspect;

  // For stereo
  float eyeSep;
};
}
