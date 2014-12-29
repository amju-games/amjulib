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
  virtual void Init();
  
  virtual void Enable(uint32 flags);
  virtual void Disable(uint32 flags);

  virtual void DrawTriList(const AmjuGL::Tris& tris);

  virtual void DrawIndexedTriList(
    const AmjuGL::Verts& verts,
    const AmjuGL::IndexedTriList& indexes);

  virtual void SetColour(float r, float g, float b, float a = 1.0f);

  virtual void SetMatrixMode(AmjuGL::MatrixMode);

  virtual void SetIdentity();

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

  virtual void SetOrthoProjection();
	
  virtual void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ);
		
	
  virtual void DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2);
		
  virtual void SetTexture(
    AmjuGL::TextureHandle*,
	AmjuGL::TextureType,
	AmjuGL::TextureDepth,
	int width,
	int height,
	uint8* data);
	
  virtual void SetTextureMode(AmjuGL::TextureMode tm);
	
  virtual void GetScreenshot(unsigned char* buffer, int w, int h);
	
  virtual bool CreateWindow(AmjuGLWindowInfo*);

  virtual void Flip();
  virtual void SetPerspectiveProjection(
    float fov, float aspectRatio, float nearDist, float farDist);

  virtual Shader* LoadShader(const std::string& shaderFileName);
  virtual void UseShader(Shader*);

  virtual void SetTextureType(AmjuGL::TextureType tt);
    
  virtual void DrawLighting(
    const AmjuGL::LightColour& globalAmbient,
    const AmjuGL::LightColour& lightAmbient,
    const AmjuGL::LightColour& lightDiffuse,
    const AmjuGL::LightColour& lightSpecular,
    const AmjuGL::Vec3& lightPos);
  
  virtual Drawable* Create(int drawableTypeId);
};
}

#endif // AMJU_IOS

