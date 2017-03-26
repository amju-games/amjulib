/*
Amju Games source code (c) Copyright Jason Colman 2010
*/

#pragma once

#if defined(AMJU_USE_ES2)

#include "AmjuGL-OpenGL-Base.h"

namespace Amju
{
class AmjuGLOpenGLES2 : public AmjuGLOpenGLBase
{
public:
  AmjuGLOpenGLES2();
  virtual void Init() override;
  
  virtual void Enable(uint32 flags) override;
  virtual void Disable(uint32 flags) override;

  virtual void DrawTriList(const AmjuGL::Tris& tris) override;

  virtual void DrawIndexedTriList(
    const AmjuGL::Verts& verts,
    const AmjuGL::IndexedTriList& indexes) override;

  virtual void SetColour(float r, float g, float b, float a = 1.0f) override;

  virtual void SetMatrixMode(AmjuGL::MatrixMode) override;

  virtual void SetIdentity() override;

  virtual void PushMatrix() override;
  virtual void PopMatrix() override;

  // Translate/Scale/Rotate top of current matrix stack
  virtual void Translate(float x, float y, float z) override;
  virtual void Scale(float x, float y, float z) override;
  virtual void RotateX(float degs) override;
  virtual void RotateY(float degs) override;
  virtual void RotateZ(float degs) override;

  // Get the current value of the given matrix
  virtual void GetMatrix(AmjuGL::MatrixMode, float result[16]) override;

  virtual void MultMatrix(const float matrix[16]) override;

  virtual void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ) override;
		
	
  virtual void DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2) override;
		
  virtual void SetTexture(
    AmjuGL::TextureHandle*,
    AmjuGL::TextureType,
    AmjuGL::TextureDepth,
    int width,
    int height,
    uint8* data) override;
	
  virtual void SetTextureMode(AmjuGL::TextureMode tm) override;
	
  virtual void GetScreenshot(unsigned char* buffer, int x, int y, int w, int h) override;
	
  virtual bool CreateWindow(AmjuGLWindowInfo*) override;

  virtual void Flip() override;
  
  virtual void SetPerspectiveProjection(
    float fov, float aspectRatio, float nearDist, float farDist) override;

  virtual void SetOrthoProjection(float left, float right, float top, float bottom, float near, float far) override;
  

  virtual Shader* LoadShader(const std::string& shaderFileName) override;
  virtual void UseShader(Shader*) override;

  virtual void SetTextureType(AmjuGL::TextureType tt) override;
    
  virtual void DrawLighting(
    const AmjuGL::LightColour& globalAmbient,
    const AmjuGL::LightColour& lightAmbient,
    const AmjuGL::LightColour& lightDiffuse,
    const AmjuGL::LightColour& lightSpecular,
    const AmjuGL::Vec3& lightPos) override;
  
  virtual Drawable* Create(int drawableTypeId) override;
};
}

#endif // AMJU_IOS

