#pragma once

#include "DrawDecorator.h"
#include "Camera.h"

namespace Amju
{
// Draw a stereo view for HMD etc.
class StereoDraw : public DrawDecorator
{
public:
  StereoDraw();

  void SetCamera(const Camera& cam) { m_camera = cam; }
  void SetIsStereo(bool s) { m_isStereo = s; }
  bool IsStereo() const { return m_isStereo; }

  virtual bool Init() override { return true; }
  virtual void Draw() override;

private:
  Camera m_camera;
  bool m_isStereo;
};
}

