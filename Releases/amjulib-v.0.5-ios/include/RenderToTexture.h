#pragma once

#include "Drawable.h"

namespace Amju
{
class RenderToTexture : public Drawable
{
public:
  enum { DRAWABLE_TYPE_ID = 10 };

  RenderToTexture();

  // Set what to render, depth and/or colour. Default value is AMJU_RENDER_COLOUR
  static const int AMJU_RENDER_COLOUR = 0x01;
  static const int AMJU_RENDER_DEPTH = 0x02; 
  void SetRenderFlags(int renderFlags);

  void SetSize(int w, int h);

  // Draw scene between Begin() and End() calls. Then you can use the texture.
  // Switch to texture render target
  virtual bool Begin() = 0;

  // Switch back to regular render target
  virtual bool End() = 0;
 
  // "Drawing" the RenderToTexture object means we use the texture
  //  for what we draw next (e.g. draw full screen quad)

  virtual void Draw() override { UseThisTexture(); }

  // Make the texture(s) active. There could be 1 texture (depth or colour)
  //  or 2 textures if both are set in SetRenderFlags.
  virtual void UseThisTexture() = 0;

  // Show the off-screen render target as an on-screen quad, for
  //  debugging
  virtual void DebugDraw() {}

protected:
  int m_width, m_height; // pixels - size of texture to which we render
  
  // Viewport before switching to this render target, so we revert in End()
  int m_vpX, m_vpY, m_vpW, m_vpH;

  int m_renderFlags;
};
}

