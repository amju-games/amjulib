#ifndef AMJU_GL_NULL_H
#define AMJU_GL_NULL_H

#include "AmjuGL-Impl.h"
#include "ShaderNull.h"

namespace Amju
{
class AmjuGLNull : public AmjuGLImpl
{
public:
  virtual void Init() {}
  virtual bool CreateWindow(AmjuGLWindowInfo*) { return true; }
  virtual void BeginScene() {}
  virtual void EndScene() {}
  virtual void Flip() {}
  virtual void Viewport(int x, int y, int w, int h) {}
  virtual void InitFrame(float clearR, float clearG, float clearB) {}
  virtual void SetPerspectiveProjection(
    float fov, float aspectRatio, float nearDist, float farDist) {}
  virtual void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ) {}
  virtual void SetColour(float r, float g, float b, float a = 1.0f) {}
  virtual void DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2) {}
  virtual void DrawTriList(const AmjuGL::Tris& tris) {}
  virtual void DrawIndexedTriList(
    const AmjuGL::Verts& verts,
    const AmjuGL::IndexedTriList& indexes) {}
  virtual void SetMatrixMode(AmjuGL::MatrixMode) {}
  virtual void SetIdentity() {}
  virtual void PushMatrix() {}
  virtual void PopMatrix() {}
  virtual void Translate(float x, float y, float z) {}
  virtual void Scale(float x, float y, float z) {}
  virtual void RotateX(float degs) {}
  virtual void RotateY(float degs) {}
  virtual void RotateZ(float degs) {}
  virtual void GetMatrix(AmjuGL::MatrixMode, float result[16]) {}
  virtual void MultMatrix(const float matrix[16]) {}
  virtual void PushAttrib(uint32 attrib) {}
  virtual void PopAttrib() {}
  virtual void Enable(uint32 flags) {}
  virtual void Disable(uint32 flags) {}
  virtual void DestroyTextureHandle(AmjuGL::TextureHandle*) {}
  virtual void SetTexture(
    AmjuGL::TextureHandle* th, 
    AmjuGL::TextureType, 
    AmjuGL::TextureDepth, 
    int width, 
    int height, 
    uint8* data) 
  { 
    *th = 1; // Give texture handle a valid value, it is checked
  }
  virtual void UseTexture(AmjuGL::TextureHandle) {}
  virtual void SetTextureType(AmjuGL::TextureType) {}
  virtual void SetTextureMode(AmjuGL::TextureMode) {}
  virtual void SetTextureFilter(AmjuGL::TextureFilter) {}
  virtual void GetScreenshot(unsigned char* buffer, int w, int h) {}
  virtual void DrawLighting(
    const AmjuGL::LightColour& globalAmbient, 
    const AmjuGL::LightColour& lightAmbient,
    const AmjuGL::LightColour& lightDiffuse,
    const AmjuGL::LightColour& lightSpecular,
    const AmjuGL::Vec3& lightPos) {}

  virtual Shader* LoadShader(const std::string& shaderFileName) { return new ShaderNull; }

  virtual Drawable* Create(int) { return 0; }
}; // class 

}

#endif
