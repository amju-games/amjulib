/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#ifndef AMJU_GL_DX9_H_INCLUDED
#define AMJU_GL_DX9_H_INCLUDED

#include "AmjuGL-Impl.h"
#include "Colour.h"
#include <d3d9.h>

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace Amju
{
class AmjuGLDX9 : public AmjuGLImpl
{
public:
  // Ideally private but we access this in some non-member functions in 
  //  CreateWindowDX9.cpp
  static LPDIRECT3DDEVICE9 dd;

  AmjuGLDX9(WNDPROC wndproc);

  // Call once at app startup
  virtual void Init();

  // Call to create window
  virtual bool CreateWindow(AmjuGLWindowInfo*);

  // Call before drawing anything
  virtual void BeginScene();

  // Call when all drawing finished: Impl must swap buffers
  virtual void EndScene();

  // Call to flip back/front buffers
  virtual void Flip();

  // Set viewport as screen coords
  virtual void Viewport(int x, int y, int w, int h);

  // Call at start of drawing every frame.
  // Specify clear colour
  virtual void InitFrame(float clearR, float clearG, float clearB);

  // Set up projection matrix
  virtual void SetPerspectiveProjection(float fov, float aspectRatio, 
    float nearDist, float farDist);

  // Set 'camera': give position of eye, target position and Up vector
  virtual void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ);

  // Set the current colour
  virtual void SetColour(float r, float g, float b, float a = 1.0f);

  // Draw line in current colour between 2 absolute coords
  virtual void DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2);

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

  // Lighting
  virtual void DrawLighting(
    const AmjuGL::LightColour& globalAmbient, 
    const AmjuGL::LightColour& lightAmbient,
    const AmjuGL::LightColour& lightDiffuse,
    const AmjuGL::LightColour& lightSpecular,
    const AmjuGL::Vec3& lightPos);

  virtual Shader* LoadShader(const std::string& shaderFileName);

private:
  WNDPROC m_wndproc;
}; // class 

} // namespace Amju

#endif

