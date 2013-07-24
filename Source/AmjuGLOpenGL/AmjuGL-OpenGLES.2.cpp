/*
Amju Games source code (c) Copyright Jason Colman 2010
*/

#ifdef IPHONE

#include <AmjuFirst.h>
#include <math.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
extern "C"
{
#include <GLKit/GLKMatrix4.h>
}
#include <AmjuGL.h>
#include "AmjuGL-OpenGLES.2.h"
#include <AmjuFinal.h>

#define GLint int
#define GLfloat float
#define GLdouble double
#define GLboolean int
#define GL_FALSE 0
#define GL_TRUE 1
#define GLAPIENTRY
#define MEMCPY memcpy
#define __glPi 3.14159265358979323846

namespace Amju
{
  static GLKMatrix4 s_matrices[3]; // index with AmjuGL::MatrixMode
  static AmjuGL::MatrixMode s_matrixMode;
    
// Remember the current texture type. If sphere mapped, no need to send
// texture coords to the graphics card.
// TODO Not very useful for ES - but can use to check that unsupported modes are not used
static AmjuGL::TextureType s_tt = AmjuGL::AMJU_TEXTURE_REGULAR;
	

bool AmjuGLOpenGLES2::CreateWindow(AmjuGLWindowInfo*)
{
  return true;
}
	
void AmjuGLOpenGLES2::Flip()
{
}	

Shader* AmjuGLOpenGLES2::LoadShader(const std::string& shaderFileName)
{
  return 0;
}
	
void AmjuGLOpenGLES2::SetPerspectiveProjection(float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;

  // Expect current matrix mode to be perspective
  s_matrices[AmjuGL::AMJU_PROJECTION_MATRIX] = GLKMatrix4Multiply(
    s_matrices[AmjuGL::AMJU_PROJECTION_MATRIX],
    GLKMatrix4MakePerspective(fov, aspectRatio, nearDist, farDist));
}

void AmjuGLOpenGLES2::SetOrthoProjection()
{
	AMJU_CALL_STACK;
	
	// TODO
}

void AmjuGLOpenGLES2::LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ)
{
	AMJU_CALL_STACK;
	
    // Expect current matrix mode to be mview
    s_matrices[AmjuGL::AMJU_MODELVIEW_MATRIX] = GLKMatrix4Multiply(
      s_matrices[AmjuGL::AMJU_MODELVIEW_MATRIX],
      GLKMatrix4MakeLookAt(eyeX, eyeY, eyeZ, x, y, z, upX, upY, upZ));
}

void AmjuGLOpenGLES2::SetTextureMode(AmjuGL::TextureType tt)
{
	AMJU_CALL_STACK;
	
	s_tt = tt;
	
	if (tt == AmjuGL::AMJU_TEXTURE_REGULAR)
	{
		/*
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		 */
	}
	else if (tt == AmjuGL::AMJU_TEXTURE_SPHERE_MAP)
	{
		Assert(0); // Not supported for ES
	}
	
	glEnable(GL_TEXTURE_2D);
}

void AmjuGLOpenGLES2::SetTexture(
							  AmjuGL::TextureHandle* th, 
							  AmjuGL::TextureType tt, 
							  AmjuGL::TextureDepth d, 
							  int width, 
							  int height, 
							  uint8* data)
{
	AMJU_CALL_STACK;
	
	glGenTextures(1, (GLuint*)th);
	
	glBindTexture(GL_TEXTURE_2D, *th);
	
	glEnable(GL_TEXTURE_2D);
	
	int wrapmode = GL_REPEAT;
	/*
	bool wrap = true; // TODO
	if (!wrap)
	{
		wrapmode = GL_CLAMP;
	}
	 */
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmode);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmode);
	
	// These modes only work if you have set up mipmaps I should expect!!!! Duh!!
	
	// New code: changed to use mipmaps

	
	// Mipmapping/filtering: these settings are recommended.
	// Use this - but requires mipmaps to be created ?
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	// Bad quality(?) but works if no mipmaps set up
	//glTexParameterf(GL_TEXTURE_2D, 
	//				GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D,
	//				GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Allowed values for min filter:
	// GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST,
	// GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
	
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	// Different depending on RGB or RGBA.
	if (d == AmjuGL::AMJU_RGB)
	{
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGB,
					 width,
					 height,
					 0,
					 GL_RGB,
					 GL_UNSIGNED_BYTE,
					 data);
	}
	else //if (d == RGBA)
	{
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGBA, // four components, not 3 - i.e., RGBA.
					 width,
					 height,
					 0,
					 GL_RGBA, // not just RGB.
					 GL_UNSIGNED_BYTE,
					 data);
	}
	
	// TODO Compressed textures -- "PVRT" formats
	
	// Build mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
}

void AmjuGLOpenGLES2::GetScreenshot(unsigned char* buffer, int w, int h)
{
	AMJU_CALL_STACK;

	unsigned char* myBuf = new unsigned char[4 * w * h];
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, myBuf);
	
#ifdef LANDSCAPE
	// Convert RGBA to RGB and rotate 90 degs - But TODO How to make sure it's the right way up ?
/*
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			// TODO
			buffer[(i + ((h - 1 - j) * w)) * 3 + 0] = myBuf[((w - 1 - i) * h + j) * 4 + 3];
			buffer[(i + ((h - 1 - j) * w)) * 3 + 1] = myBuf[((w - 1 - i) * h + j) * 4 + 2];
			buffer[(i + ((h - 1 - j) * w)) * 3 + 2] = myBuf[((w - 1 - i) * h + j) * 4 + 1];
		}
	}
*/

	// Rotate
	unsigned char* myBuf2 = new unsigned char[4 * w * h];
	for (int i = 0; i < w * h; i++)
	{
		int src = i;
		int dst = (i % 320) * 480 - (i / 320) + 479;
		
		memcpy(myBuf2 + dst * 4, myBuf + src * 4, 4);
	}	
	
	delete [] myBuf;
	myBuf = myBuf2;
#endif

	// Convert RGBA to RGB
	for (int i = 0; i < w * h; i++)
	{
		buffer[i * 3 + 0] = myBuf[i * 4 + 0];
		buffer[i * 3 + 1] = myBuf[i * 4 + 1];
		buffer[i * 3 + 2] = myBuf[i * 4 + 2];
	}	
	
	delete [] myBuf;
}

void AmjuGLOpenGLES2::DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2)
{
	AMJU_CALL_STACK;

	// TODO
	//Assert(0);
}
	
void AmjuGLOpenGLES2::SetTextureType(AmjuGL::TextureType tt)
{
  AMJU_CALL_STACK;
    
  // TODO - must use shader in ES
/*
  if (tt == AmjuGL::AMJU_TEXTURE_REGULAR)
  {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  }
  else if (tt == AmjuGL::AMJU_TEXTURE_SPHERE_MAP)
  {
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
  }
*/
}

static uint32 ConvertToGLFlag(uint32 flag)
{
  AMJU_CALL_STACK;

  switch (flag)
  {
//  case AmjuGL::AMJU_LIGHTING:
//    return GL_LIGHTING;
  case AmjuGL::AMJU_BLEND:
    return GL_BLEND;
  case AmjuGL::AMJU_DEPTH_READ:
    return GL_DEPTH_TEST;
  case AmjuGL::AMJU_TEXTURE_2D:
    return GL_TEXTURE_2D;
  }
  return 0;
}

void AmjuGLOpenGLES2::Enable(uint32 flag)
{
  AMJU_CALL_STACK;

  if (flag == AmjuGL::AMJU_DEPTH_WRITE)
  {
    glDepthMask(GL_TRUE);
    return;
  }

  uint32 glFlag = ConvertToGLFlag(flag);
  if (glFlag)
  {
    glEnable(glFlag);
  }
}

void AmjuGLOpenGLES2::Disable(uint32 flag)
{
  AMJU_CALL_STACK;

  if (flag == AmjuGL::AMJU_DEPTH_WRITE)
  {
    glDepthMask(GL_FALSE);
    return;
  }

  uint32 glFlag = ConvertToGLFlag(flag);
  if (glFlag)
  {
    glDisable(glFlag);
  }
}

void AmjuGLOpenGLES2::DrawTriList(const AmjuGL::Tris& tris)
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGLES2::DrawIndexedTriList(
  const AmjuGL::Verts& verts,
  const AmjuGL::IndexedTriList& indexes)
{
  AMJU_CALL_STACK;

  // TODO
}

void AmjuGLOpenGLES2::SetColour(float r, float g, float b, float a)
{
  AMJU_CALL_STACK;
}


void AmjuGLOpenGLES2::GetMatrix(AmjuGL::MatrixMode m, float result[16])
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGLES2::SetMatrixMode(AmjuGL::MatrixMode m)
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGLES2::SetIdentity()
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGLES2::PushMatrix()
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGLES2::PopMatrix()
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGLES2::Translate(float x, float y, float z)
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGLES2::Scale(float x, float y, float z)
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGLES2::RotateX(float degs)
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGLES2::RotateY(float degs)
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGLES2::RotateZ(float degs)
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGLES2::MultMatrix(const float matrix[16])
{
  AMJU_CALL_STACK;
}
    
void AmjuGLOpenGLES2::DrawLighting(
  const AmjuGL::LightColour& globalAmbient,
  const AmjuGL::LightColour& lightAmbient,
  const AmjuGL::LightColour& lightDiffuse,
  const AmjuGL::LightColour& lightSpecular,
  const AmjuGL::Vec3& lightPos)
{
  AMJU_CALL_STACK;
}    

} // namespace Amju

#endif // IPHONE

