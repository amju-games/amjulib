#pragma once

#include "Internal/OpenGL.h"

namespace Amju
{
class RenderToTextureES2
{
public:
  RenderToTextureES2();
  ~RenderToTextureES2();

  // Create texture render target
  bool Init(int w, int h);

  // Draw scene between Begin() and End() calls. Then you can use the texture.
  // Switch to texture render target
  bool Begin();

  // Switch back to regular render target
  bool End();
  
  void UseThisTexture();

private:
  GLuint m_framebuffer;
  GLuint m_tex;
  GLint m_old_fbo;
  int m_width, m_height; // pixels - size of texture to which we render
};
}

