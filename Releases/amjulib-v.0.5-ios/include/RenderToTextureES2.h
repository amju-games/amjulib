#pragma once

#if defined (AMJU_IOS) || defined (ANDROID_NDK) || defined(AMJU_USE_ES2)

#include <RenderToTexture.h>
#include "Internal/OpenGL.h"

namespace Amju
{
class RenderToTextureES2 : public RenderToTexture
{
public:
  RenderToTextureES2();
  ~RenderToTextureES2();

  // Create texture render target
  bool Init() override;

  // Draw scene between Begin() and End() calls. Then you can use the texture.
  // Switch to texture render target
  bool Begin() override;

  // Switch back to regular render target
  bool End() override;
  
  void UseThisTexture() override;

protected:
  bool InitDepth();
  bool InitColour();
  bool InitDepthColour();

private:
  GLuint m_framebuffer;
  GLuint m_tex[2]; // 0 is colour, 1 is depth, if both are requested
  GLint m_old_fbo;
};
}

#endif

