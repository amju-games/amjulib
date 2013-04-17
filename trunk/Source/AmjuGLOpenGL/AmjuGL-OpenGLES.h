/*
Amju Games source code (c) Copyright Jason Colman 2010
*/

#include "AmjuGL-OpenGL-Base.h"

namespace Amju
{
class AmjuGLOpenGLES : public AmjuGLOpenGLBase
{
public:
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
	
	virtual void SetTextureMode(AmjuGL::TextureType tt);
	
	virtual void GetScreenshot(unsigned char* buffer, int w, int h);
	
	virtual bool CreateWindow(AmjuGLWindowInfo*);
	virtual void Flip();
	virtual void SetPerspectiveProjection(
        float fov, float aspectRatio, float nearDist, float farDist);
	virtual Shader* LoadShader(const std::string& shaderFileName);
	virtual void SetTextureType(AmjuGL::TextureType tt);
};
}
