#pragma once

#if defined (AMJU_IOS) || defined (ANDROID_NDK) || defined(AMJU_USE_ES2)

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

#endif

