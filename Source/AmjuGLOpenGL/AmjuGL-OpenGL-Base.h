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
class AmjuGLOpenGLBase : public AmjuGLImpl
{
public:
  // Call before drawing anything
  virtual void BeginScene();

  // Call when all drawing finished: Impl must swap buffers
  virtual void EndScene();

  // Set viewport as screen coords
  virtual void Viewport(int x, int y, int w, int h);

  // Call once at app startup
  virtual void Init();

  // Call at start of drawing every frame.
  // Specify clear colour
  virtual void InitFrame(float clearR, float clearG, float clearB);

  // Set the current colour
  virtual void SetColour(float r, float g, float b, float a = 1.0f);

  virtual void DrawTriList(const AmjuGL::Tris& tris);

  virtual void DrawIndexedTriList(
    const AmjuGL::Verts& verts,
    const AmjuGL::IndexedTriList& indexes);

  // Set current matrix stack
  virtual void SetMatrixMode(AmjuGL::MatrixMode);

  // Set the current Matrix to the identity
  virtual void SetIdentity();

  // Push/pop current matrix stack
  virtual void PushMatrix();
  virtual void PopMatrix();

  // Translate/Scale/Rotate top of current matrix stack
  virtual void Translate(float x, float y, float z);
  virtual void Scale(float x, float y, float z);
  virtual void RotateX(float degs);
  virtual void RotateY(float degs);
  virtual void RotateZ(float degs);

  // Get the current value of the given matrix
  virtual void GetMatrix(AmjuGL::MatrixMode, float result[16]);

  virtual void MultMatrix(const float matrix[16]);

  virtual void PushAttrib(uint32 attrib);
  virtual void PopAttrib();

  virtual void Enable(uint32 flags);
  virtual void Disable(uint32 flags);

  // Call to delete Texture handle
  virtual void DestroyTextureHandle(AmjuGL::TextureHandle*);

  // Call to use a Texture once set up
  virtual void UseTexture(AmjuGL::TextureHandle);

  // Lighting
  virtual void DrawLighting(
    const AmjuGL::LightColour& globalAmbient, 
    const AmjuGL::LightColour& lightAmbient,
    const AmjuGL::LightColour& lightDiffuse,
    const AmjuGL::LightColour& lightSpecular,
    const AmjuGL::Vec3& lightPos);

  // True if shaders are supported on this machine
  static bool s_shaderSupport;

}; // class 
} // namespace Amju

#endif

