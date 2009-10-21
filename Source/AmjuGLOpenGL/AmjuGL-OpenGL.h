/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#ifndef AMJU_GL_H_OPENGL_INCLUDED
#define AMJU_GL_H_OPENGL_INCLUDED

#include <string>
#include <vector>
#include "AmjuGL-Impl.h"
#include "AmjuGL-OpenGL-Base.h"
#include "AmjuGLWindowInfo.h"

namespace Amju
{
class AmjuGLOpenGL : public AmjuGLOpenGLBase
{
public:
  // Call once at app startup
  virtual void Init();

  // Call to create window
  virtual bool CreateWindow(AmjuGLWindowInfo*);

  // Call when all drawing finished
  virtual void Flip();

  // Call at start of drawing every frame.
  // Specify clear colour
  virtual void InitFrame(float clearR, float clearG, float clearB);

  // Set up projection matrix, by specifying field-of-view and aspect ratio.
  // Near and far planes are currently hardcoded.
  virtual void SetPerspectiveProjection(
    float fov, float aspectRatio, float near, float far);

  // Set 'camera': give position of eye, target position and Up vector
  virtual void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ);

  // Draw line in current colour between 2 absolute coords
  virtual void DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2);

  // Draw quad with the 4 vertices
  virtual void DrawQuad(AmjuGL::Vert* verts);

  virtual void DrawQuadList(const AmjuGL::Quads& quads);

  virtual void DrawTriList(const AmjuGL::Tris& tris);

  virtual void DrawIndexedTriList(
    const AmjuGL::Verts& verts,
    const AmjuGL::IndexedTriList& indexes);

  // Call to delete Texture handle
  virtual void DestroyTextureHandle(AmjuGL::TextureHandle*);

  virtual void SetTexture(
    AmjuGL::TextureHandle*, 
    AmjuGL::TextureType, 
    AmjuGL::TextureDepth, 
    int width, 
    int height, 
    uint8* data);

  // Call to use a Texture once set up
  virtual void UseTexture(AmjuGL::TextureHandle);

  virtual void SetTextureMode(AmjuGL::TextureType tt);

  // Copy screen into buffer  - which should be allocated by caller.
  virtual void GetScreenshot(unsigned char* buffer, int w, int h);

  virtual Shader* LoadShader(const std::string& shaderFileName);

//private:
  // True if shaders are supported on this machine
  static bool s_shaderSupport;

}; // class 

} // namespace Amju

#endif

