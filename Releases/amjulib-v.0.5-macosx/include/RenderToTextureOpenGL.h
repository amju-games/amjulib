#pragma once

#include <RenderToTexture.h>
#include "Internal/OpenGL.h"

namespace Amju
{
class RenderToTextureOpenGL : public RenderToTexture
{
public:
  RenderToTextureOpenGL();
  ~RenderToTextureOpenGL();

  // Create texture render target
  bool Init() override;

  // Draw scene between Begin() and End() calls. Then you can use the texture.
  // Switch to texture render target
  bool Begin() override;

  // Switch back to regular render target
  bool End() override;
  
  void UseThisTexture() override;

  void DebugDraw() override;

protected:
  bool InitDepth();
  bool InitColour();
  bool InitDepthColour();

protected:
  GLuint m_framebuffer;
  GLuint m_tex[2]; // 0 is colour, 1 is depth
  GLint m_old_fbo;
};
}


