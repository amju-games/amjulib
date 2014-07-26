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

private:
  GLuint m_framebuffer;
  GLuint m_tex;
  GLint m_old_fbo;
};
}


