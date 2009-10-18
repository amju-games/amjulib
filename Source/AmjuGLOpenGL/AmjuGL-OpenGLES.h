/*
  */

#include "AmjuGL-OpenGL-Base.h"

namespace Amju
{
class AmjuGLOpenGLES : public AmjuGLOpenGLBase
{
public:
	virtual void SetPerspectiveProjection(float fov, float aspectRatio);
	
	virtual void SetOrthoProjection();
	
	virtual void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ);
		
	virtual void DrawQuad(AmjuGL::Vert* verts);
	
	virtual void DrawQuadList(const AmjuGL::Quads& quads);
	
	virtual void DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2);
		
	virtual void SetTexture(
							AmjuGL::TextureHandle*, 
							AmjuGL::TextureType, 
							AmjuGL::TextureDepth, 
							int width, 
							int height, 
							uint8* data);
	
	virtual void SetTextureMode(AmjuGL::TextureType tt);
	
	virtual AmjuGL::Vec3 MouseToWorld(int mouseX, int mouseY);
	
	virtual void GetScreenshot(unsigned char* buffer, int w, int h);
		
};
}