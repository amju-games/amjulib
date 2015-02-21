/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#ifndef AMJU_GL_H_IMPL_INCLUDED
#define AMJU_GL_H_IMPL_INCLUDED

#include <vector>
#include "AmjuGL.h"
#include "RCPtr.h"

namespace Amju
{
class AmjuGLImpl : public RefCounted
{
public:
  virtual ~AmjuGLImpl() {}

  // Call once at app startup
  virtual void Init() = 0;

  // Call to create window
  virtual bool CreateWindow(AmjuGLWindowInfo*) = 0;

  // Call before drawing anything
  virtual void BeginScene() = 0;

  // Call when all drawing finished
  virtual void EndScene() = 0;

  // Call to flip back/front buffers
  virtual void Flip() = 0;

  // Set viewport as screen coords
  virtual void Viewport(int x, int y, int w, int h) = 0;

  // Call at start of drawing every frame.
  // Specify clear colour
  virtual void InitFrame(float clearR, float clearG, float clearB) = 0;

  // Set up projection matrix, by specifying field-of-view and aspect ratio.
  virtual void SetPerspectiveProjection(
    float fov, float aspectRatio, float nearDist, float farDist) = 0;

  // Set 'camera': give position of eye, target position and Up vector
  virtual void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ) = 0;

  // Set the current colour
  virtual void SetColour(float r, float g, float b, float a = 1.0f) = 0;

  // Draw line in current colour between 2 absolute coords
  virtual void DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2) = 0;

  // Draw list of triangles
  virtual void DrawTriList(const AmjuGL::Tris& tris) = 0;

  virtual void DrawIndexedTriList(
    const AmjuGL::Verts& verts,
    const AmjuGL::IndexedTriList& indexes) = 0;

  // Set current matrix stack
  virtual void SetMatrixMode(AmjuGL::MatrixMode) = 0;

  // Set the current Matrix to the identity
  virtual void SetIdentity() = 0;

  // Push/pop current matrix stack
  virtual void PushMatrix() = 0;
  virtual void PopMatrix() = 0;

  // Translate/Scale/Rotate top of current matrix stack
  virtual void Translate(float x, float y, float z) = 0;
  virtual void Scale(float x, float y, float z) = 0;
  virtual void RotateX(float degs) = 0;
  virtual void RotateY(float degs) = 0;
  virtual void RotateZ(float degs) = 0;

  // Get the current value of the given matrix
  virtual void GetMatrix(AmjuGL::MatrixMode, float result[16]) = 0;

  virtual void MultMatrix(const float matrix[16]) = 0;

  virtual void PushAttrib(uint32 attrib) = 0;
  virtual void PopAttrib() = 0;

  virtual void Enable(uint32 flags) = 0;
  virtual void Disable(uint32 flags) = 0;


  // Call to delete Texture handle
  virtual void DestroyTextureHandle(AmjuGL::TextureHandle*) = 0;

  virtual void SetTexture(
    AmjuGL::TextureHandle*, 
    AmjuGL::TextureType, 
    AmjuGL::TextureDepth, 
    int width, 
    int height, 
    uint8* data) = 0;

  virtual void UpdateTexture(const AmjuGL::TextureHandle th, int x, int y, int w, int h, const uint8* data) = 0;

  // Call to use a Texture once set up
  virtual void UseTexture(AmjuGL::TextureHandle, int textureUnitId) = 0;

  virtual void SetTextureType(AmjuGL::TextureType tt) = 0;
  virtual void SetTextureMode(AmjuGL::TextureMode tm) = 0;
  virtual void SetTextureFilter(AmjuGL::TextureFilter tf) = 0;

  // Copy screen into buffer  - which should be allocated by caller.
  virtual void GetScreenshot(unsigned char* buffer, int w, int h) = 0;

  // Lighting
  virtual void DrawLighting(
    const AmjuGL::LightColour& globalAmbient, 
    const AmjuGL::LightColour& lightAmbient,
    const AmjuGL::LightColour& lightDiffuse,
    const AmjuGL::LightColour& lightSpecular,
    const AmjuGL::Vec3& lightPos) = 0;

  virtual Shader* LoadShader(const std::string& shaderFileName) = 0;
  virtual void UseShader(Shader*) = 0;

  virtual Drawable* Create(int) = 0;
}; // class 
} // namespace Amju

#endif

