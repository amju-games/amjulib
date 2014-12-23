#pragma once

#include "DrawDecorator.h"
#include "Camera.h"

namespace Amju
{
// Draw a stereo view for HMD etc.
class StereoDraw : public DrawDecorator
{
public:
  void SetCamera(const Camera& cam) { m_camera = cam; }

  virtual bool Init() override { return true; }
  virtual void Draw() override;

private:
  Camera m_camera;
};
}

