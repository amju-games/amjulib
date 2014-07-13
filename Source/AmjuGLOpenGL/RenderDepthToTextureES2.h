#pragma once

namespace Amju
{
class RenderDepthToTexture
{
public:
  RenderDepthToTextureES2();
  ~RenderDepthToTextureES2();

  // Create texture render target
  bool Init(int w, int h);

  // Draw scene between Begin() and End() calls. Then you can use the texture.
  // Switch to texture render target
  bool Begin();

  // Switch back to regular render target
  bool End();
};
}

