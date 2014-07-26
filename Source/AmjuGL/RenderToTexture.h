#pragma once

#include "Drawable.h"

namespace Amju
{
class RenderToTexture : public Drawable
{
public:
  enum { DRAWABLE_TYPE_ID = 10 };

  RenderToTexture();

  void SetSize(int w, int h);

  // Draw scene between Begin() and End() calls. Then you can use the texture.
  // Switch to texture render target
  virtual bool Begin() = 0;

  // Switch back to regular render target
  virtual bool End() = 0;
 
  // "Drawing" the RenderToTexture object means we use the texture
  //  for what we draw next (e.g. draw full screen quad)

  virtual void Draw() override { UseThisTexture(); } 
  virtual void UseThisTexture() = 0;

protected:
  int m_width, m_height; // pixels - size of texture to which we render
  
  // Viewport before switching to this render target, so we revert in End()
  int m_vpX, m_vpY, m_vpW, m_vpH;
};
}

