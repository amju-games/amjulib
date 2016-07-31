/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#ifndef AMJU_GL_H_OPENGL_BASE_INCLUDED
#define AMJU_GL_H_OPENGL_BASE_INCLUDED

#include <string>
#include <vector>
#include "AmjuGL-Impl.h"

namespace Amju
{
void CheckOpenGLError(const char* stmt, const char* fname, int line);

#define GL_CHECK(stmt) do { \
stmt; \
CheckOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)
  
class AmjuGLOpenGLBase : public AmjuGLImpl
{
public:
  // Call before drawing anything
  virtual void BeginScene();

  // Call when all drawing finished: Impl must swap buffers
  virtual void EndScene();

  // Set viewport as screen coords
  virtual void Viewport(int x, int y, int w, int h);

  // Call once at app startup, once GL context created
  virtual void Init();

  // Call at start of drawing every frame.
  // Specify clear colour
  virtual void InitFrame(float clearR, float clearG, float clearB);

  virtual void PushAttrib(uint32 attrib);
  virtual void PopAttrib();

  // Call to delete Texture handle
  virtual void DestroyTextureHandle(AmjuGL::TextureHandle*);

  // Call to use a Texture once set up
  virtual void UseTexture(AmjuGL::TextureHandle, int textureUnitId);

  virtual void SetTextureMode(AmjuGL::TextureMode tm);
  virtual void SetTextureFilter(AmjuGL::TextureFilter tf);

  // TODO can do this in ES?
  virtual void UpdateTexture(const AmjuGL::TextureHandle th, int x, int y, int w, int h, const uint8* data);
 
protected:
  friend class GLShader;
  // True if shaders are supported on this machine
  static bool s_shaderSupport;  
}; // class
} // namespace Amju

#endif

