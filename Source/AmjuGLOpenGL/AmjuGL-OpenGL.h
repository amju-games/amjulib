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

#undef CreateWindow

namespace Amju
{
class AmjuGLOpenGL : public AmjuGLOpenGLBase
{
public:
  typedef bool (*WindowCreateFunc)(AmjuGLWindowInfo*);
  AmjuGLOpenGL(WindowCreateFunc);

  // Call once at app startup
  virtual void Init();

  // Call to create window
  virtual bool CreateWindow(AmjuGLWindowInfo*);

  // Call when all drawing finished
  virtual void Flip();

  // Set up projection matrix, by specifying field-of-view and aspect ratio.
  // Near and far planes are currently hardcoded.
  virtual void SetPerspectiveProjection(
    float fov, float aspectRatio, float near, float far);

  // Set 'camera': give position of eye, target position and Up vector
  virtual void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ);

  // Draw line in current colour between 2 absolute coords
  virtual void DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2);

  virtual void DrawTriList(const AmjuGL::Tris& tris);

  virtual void DrawIndexedTriList(
    const AmjuGL::Verts& verts,
    const AmjuGL::IndexedTriList& indexes);

  virtual void SetTexture(
    AmjuGL::TextureHandle*, 
    AmjuGL::TextureType, 
    AmjuGL::TextureDepth, 
    int width, 
    int height, 
    uint8* data);

  // Copy screen into buffer  - which should be allocated by caller.
  virtual void GetScreenshot(unsigned char* buffer, int w, int h);

  virtual Shader* LoadShader(const std::string& shaderFileName);

  virtual void SetTextureType(AmjuGL::TextureType tt);
}; // class 

} // namespace Amju

#endif

