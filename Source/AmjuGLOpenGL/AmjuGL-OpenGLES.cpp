/*
Amju Games source code (c) Copyright Jason Colman 2010
*/

#include "AmjuFirst.h"
#include <math.h>
#include <AmjuGL.h>
#include "AmjuGL-OpenGLES.h"
#include "OpenGL.h"
#include "AmjuFinal.h"

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
// Remember the current texture type. If sphere mapped, no need to send
// texture coords to the graphics card.
// TODO Not very useful for ES - but can use to check that unsupported modes are not used
static AmjuGL::TextureType s_tt = AmjuGL::AMJU_TEXTURE_REGULAR;
	
/*	
static void __gluMakeIdentityd(GLdouble m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}
*/

static void __gluMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}


static void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLfloat m[4][4];
    double sine, cotangent, deltaZ;
    double radians = fovy / 2 * __glPi / 180;
	
    deltaZ = zFar - zNear;
    sine = sin(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
		return;
    }
    cotangent = cos(radians) / sine;
	
    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = cotangent / aspect;
    m[1][1] = cotangent;
    m[2][2] = -(zFar + zNear) / deltaZ;
    m[2][3] = -1;
    m[3][2] = -2 * zNear * zFar / deltaZ;
    m[3][3] = 0;
    glMultMatrixf(&m[0][0]);
}

static void normalize(float v[3])
{
    float r;
	
    r = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
    if (r == 0.0) return;
	
    v[0] /= r;
    v[1] /= r;
    v[2] /= r;
}

static void cross(float v1[3], float v2[3], float result[3])
{
    result[0] = v1[1]*v2[2] - v1[2]*v2[1];
    result[1] = v1[2]*v2[0] - v1[0]*v2[2];
    result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

static void  gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
		  GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
		  GLdouble upz)
{
    float forward[3], side[3], up[3];
    GLfloat m[4][4];
	
    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;
	
    up[0] = upx;
    up[1] = upy;
    up[2] = upz;
	
    normalize(forward);
	
    /* Side = forward x up */
    cross(forward, up, side);
    normalize(side);
	
    /* Recompute up as: up = side x forward */
    cross(side, forward, up);
	
    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];
	
    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];
	
    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];
	
    glMultMatrixf(&m[0][0]);
    glTranslatef(-eyex, -eyey, -eyez);
}

bool AmjuGLOpenGLES::CreateWindow(AmjuGLWindowInfo*)
{
  return true;
}
	

void AmjuGLOpenGLES::Flip()
{
}	

Shader* AmjuGLOpenGLES::LoadShader(const std::string& shaderFileName)
{
  return 0;
}
	
void AmjuGLOpenGLES::SetPerspectiveProjection(float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;
	
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  // not: glMatrixMode(GL_PROJECTION);

  AmjuGL::SetIdentity();
  gluPerspective(fov, aspectRatio, nearDist, farDist);
}

void AmjuGLOpenGLES::SetOrthoProjection()
{
	AMJU_CALL_STACK;
	
	// TODO
}

void AmjuGLOpenGLES::LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ)
{
	AMJU_CALL_STACK;
	
	gluLookAt(eyeX, eyeY, eyeZ, // origin - player coords
			  x, y, z, // point in direction we want to look
			  upX, upY, upZ /* 'Up' vector */);
}

void AmjuGLOpenGLES::SetTextureMode(AmjuGL::TextureType tt)
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

void AmjuGLOpenGLES::SetTexture(
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
	//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 
	
	// Bad quality(?) but works if no mipmaps set up
	glTexParameterf(GL_TEXTURE_2D, 
					GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,
					GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Allowed values for min filter:
	// GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST,
	// GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
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
	
	/* TODO
	 
	// Build mipmaps 
	if (d == AmjuGL::RGB)
	{
		gluBuild2DMipmaps(
						  GL_TEXTURE_2D,
						  GL_RGB,
						  width,
						  height,
						  GL_RGB,
						  GL_UNSIGNED_BYTE,
						  data);
	}
	else
	{
		gluBuild2DMipmaps(
						  GL_TEXTURE_2D,
						  GL_RGBA,
						  width,
						  height,
						  GL_RGBA,
						  GL_UNSIGNED_BYTE,
						  data);
	}
	 */
}

void AmjuGLOpenGLES::GetScreenshot(unsigned char* buffer, int w, int h)
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

void AmjuGLOpenGLES::DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2)
{
	AMJU_CALL_STACK;

	// TODO
	//Assert(0);
}
	
} // namespace Amju



