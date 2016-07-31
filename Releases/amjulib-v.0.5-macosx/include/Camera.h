#pragma once

#include <Vec3.h>

namespace Amju
{
struct Camera
{
public:
  Camera();
  void SetPos(const Vec3f v) { m_pos = v; }
  void SetDir(const Vec3f v) { m_dir = v; }
  void SetUp(const Vec3f v) { m_up = v; }
  void SetNearFar(float n, float f) { m_neardist = n; m_fardist = f; }
  void SetAspect(float a) { m_aspect = a; }

  void Draw();

  float fovx() const { return m_fovy * m_aspect; }

//private:
  Vec3f m_pos;
  Vec3f m_dir;
  Vec3f m_up;
  float m_fovy;
  float m_neardist;
  float m_fardist;
  float m_aspect;
  float m_fo;

  // For stereo
  float m_eyeSep;
  float m_vpXOffset; // viewport X offset, for setting IPD
  float m_vpYOffset;
};
}
