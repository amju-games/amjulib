/*
Amju Games source code (c) Copyright Jason Colman 2010
*/

#if defined(AMJU_IOS) || defined(ANDROID_NDK)

#include <AmjuFirst.h>
#include <math.h>
#include "Internal/OpenGL.h"
#include <AmjuGL.h>
#include <TriList.h>
#include "AmjuGL-OpenGLES.1.h"
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
// Remember the current texture type. If sphere mapped, no need to send
// texture coords to the graphics card.
// TODO Not very useful for ES - but can use to check that unsupported modes are not used
static AmjuGL::TextureType s_tt = AmjuGL::AMJU_TEXTURE_REGULAR;

static DrawableFactory s_factory;
	
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

class TriListStaticES1 : public TriListStatic
{
public:
  void Draw() override
  {
    AmjuGL::DrawTriList(m_tris);
  }

  bool Init() override {}

  void Set(const AmjuGL::Tris& tris)
  {
    m_tris = tris;
  }

private:
  AmjuGL::Tris m_tris;
};

class TriListDynamicES1 : public TriListDynamic
{
public:
  void Draw() override
  {
    AmjuGL::DrawTriList(m_tris);
  }

  bool Init() override {}

  void Set(const AmjuGL::Tris& tris)
  {
    m_tris = tris;
  }

private:
  AmjuGL::Tris m_tris;
};

static Drawable* MakeStaticTriList()
{
  return new TriListStaticES1;
}

static Drawable* MakeDynamicTriList()
{
  return new TriListDynamicES1;
}

AmjuGLOpenGLES1::AmjuGLOpenGLES1()
{
  s_factory.Add(TriListStatic::DRAWABLE_TYPE_ID, MakeStaticTriList);
  s_factory.Add(TriListDynamic::DRAWABLE_TYPE_ID, MakeDynamicTriList);
}

Drawable* AmjuGLOpenGLES1::Create(int drawableTypeId)
{
  return s_factory.Create(drawableTypeId);
}

bool AmjuGLOpenGLES1::CreateWindow(AmjuGLWindowInfo*)
{
  return true;
}
	
void AmjuGLOpenGLES1::Flip()
{
}	

Shader* AmjuGLOpenGLES1::LoadShader(const std::string& shaderFileName)
{
  return 0;
}
	
void AmjuGLOpenGLES1::SetPerspectiveProjection(float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;
	
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  // not: glMatrixMode(GL_PROJECTION);

  AmjuGL::SetIdentity();
  gluPerspective(fov, aspectRatio, nearDist, farDist);
}

void AmjuGLOpenGLES1::SetOrthoProjection()
{
	AMJU_CALL_STACK;
	
	// TODO
}

void AmjuGLOpenGLES1::LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ)
{
	AMJU_CALL_STACK;
	
	gluLookAt(eyeX, eyeY, eyeZ, // origin - player coords
			  x, y, z, // point in direction we want to look
			  upX, upY, upZ /* 'Up' vector */);
}

void AmjuGLOpenGLES1::SetTextureMode(AmjuGL::TextureType tt)
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

void AmjuGLOpenGLES1::SetTexture(
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
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 
	
	// Bad quality(?) but works if no mipmaps set up
	//glTexParameterf(GL_TEXTURE_2D, 
	//				GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D,
	//				GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
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
	
	// Build mipmaps..????
    //glGenerateMipmap(GL_TEXTURE_2D);
    
    /*
	if (d == AmjuGL::AMJU_RGB)
	{
		__gluBuild2DMipmaps(
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
		__gluBuild2DMipmaps(
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

void AmjuGLOpenGLES1::GetScreenshot(unsigned char* buffer, int w, int h)
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


static uint32 ConvertToGLFlag(uint32 flag)
{
  AMJU_CALL_STACK;

  switch (flag)
  {
  case AmjuGL::AMJU_LIGHTING:
    return GL_LIGHTING;
  case AmjuGL::AMJU_BLEND:
    return GL_BLEND;
  case AmjuGL::AMJU_DEPTH_READ:
    return GL_DEPTH_TEST;
  case AmjuGL::AMJU_TEXTURE_2D:
    return GL_TEXTURE_2D;
  }
  return 0;
}

void AmjuGLOpenGLES1::Enable(uint32 flag)
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

void AmjuGLOpenGLES1::Disable(uint32 flag)
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

void AmjuGLOpenGLES1::DrawTriList(const AmjuGL::Tris& tris)
{
  AMJU_CALL_STACK;

  int numTris = tris.size();

#ifndef USE_IMMEDIATE_MODE

  // Format is different for DX9 verts and OpenGL verts 
  // So we can't do
  //glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(AmjuGL::Vert), &tris[0]);
  //glDrawArrays(GL_TRIANGLES, 0, numTris * 3);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  if (s_tt == AmjuGL::AMJU_TEXTURE_REGULAR)
  {
    // Don't specify tex coords if sphere map
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(AmjuGL::Vert), &(tris[0].m_verts[0].m_u));
  }

  glVertexPointer(3, GL_FLOAT, sizeof(AmjuGL::Vert), &(tris[0].m_verts[0].m_x));
  glNormalPointer(GL_FLOAT, sizeof(AmjuGL::Vert), &(tris[0].m_verts[0].m_nx)); 

  glDrawArrays(GL_TRIANGLES, 0, numTris * 3);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

#else // USE_IMMEDIATE_MODE

  glBegin(GL_TRIANGLES);
  for (int i = 0; i < numTris; i++)
  {
      const AmjuGL::Tri& tri = tris[i];
      for (int j = 0; j < 3; j++)
      {
        const AmjuGL::Vert& vert = tri.m_verts[j];

        // Apparently this is wrong for some vertices.
#ifdef LEAFDATA_DEBUG
std::cout << "LD2: " << m_name.c_str()
  << " Tri:" << i << " vert:" << j
  << " u:" << vert.m_u << " v:" << vert.m_v
  << "\n";
#endif

        // NB Order of these calls in important!
        glTexCoord2f(vert.m_u, vert.m_v);
        glNormal3fv(&vert.m_nx);
        glVertex3fv(&vert.m_x);
      }
  }
  glEnd();
#endif // USE_IMMEDIATE_MODE
}

void AmjuGLOpenGLES1::DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2)
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGLES1::DrawIndexedTriList(
  const AmjuGL::Verts& verts,
  const AmjuGL::IndexedTriList& indexes)
{
  AMJU_CALL_STACK;

  // TODO
}

void AmjuGLOpenGLES1::SetTextureType(AmjuGL::TextureType tt)
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

void AmjuGLOpenGLES1::SetColour(float r, float g, float b, float a)
{
  AMJU_CALL_STACK;

  glColor4f(r, g, b, a);
}


void AmjuGLOpenGLES1::GetMatrix(AmjuGL::MatrixMode m, float result[16])
{
  AMJU_CALL_STACK;

  switch (m)
  {
  case AmjuGL::AMJU_MODELVIEW_MATRIX:
    glGetFloatv(GL_MODELVIEW_MATRIX, result);
    return;
  case AmjuGL::AMJU_PROJECTION_MATRIX:
    glGetFloatv(GL_PROJECTION_MATRIX, result);
    return;
  case AmjuGL::AMJU_TEXTURE_MATRIX:
    // Allowed ?
    glGetFloatv(GL_TEXTURE_MATRIX, result);
    return;
  default:
    Assert(0);
  }
}

void AmjuGLOpenGLES1::SetMatrixMode(AmjuGL::MatrixMode m)
{
  AMJU_CALL_STACK;

  switch (m)
  {
  case AmjuGL::AMJU_MODELVIEW_MATRIX:
    glMatrixMode(GL_MODELVIEW);
    return;    

  case AmjuGL::AMJU_PROJECTION_MATRIX:
    glMatrixMode(GL_PROJECTION);
    return;

  case AmjuGL::AMJU_TEXTURE_MATRIX:
    glMatrixMode(GL_TEXTURE);
    return;

  default:
    // TODO Assert/print error
    break;
  }
}

void AmjuGLOpenGLES1::SetIdentity()
{
  AMJU_CALL_STACK;

  glLoadIdentity();
}

void AmjuGLOpenGLES1::PushMatrix()
{
  AMJU_CALL_STACK;

  glPushMatrix();
}

void AmjuGLOpenGLES1::PopMatrix()
{
  AMJU_CALL_STACK;

  glPopMatrix();
}

void AmjuGLOpenGLES1::Translate(float x, float y, float z)
{
  AMJU_CALL_STACK;

  glTranslatef(x, y, z);
}

void AmjuGLOpenGLES1::Scale(float x, float y, float z)
{
  AMJU_CALL_STACK;

  glScalef(x, y, z);
}

void AmjuGLOpenGLES1::RotateX(float degs)
{
  AMJU_CALL_STACK;

  glRotatef(degs, 1, 0, 0);
}

void AmjuGLOpenGLES1::RotateY(float degs)
{
  AMJU_CALL_STACK;

  glRotatef(degs, 0, 1, 0);
}

void AmjuGLOpenGLES1::RotateZ(float degs)
{
  AMJU_CALL_STACK;

  glRotatef(degs, 0, 0, 1);
}

void AmjuGLOpenGLES1::MultMatrix(const float matrix[16])
{
  AMJU_CALL_STACK;

  glMultMatrixf(matrix); 
}

void AmjuGLOpenGLES1::DrawLighting(
  const AmjuGL::LightColour& globalAmbient,
  const AmjuGL::LightColour& lightAmbient,
  const AmjuGL::LightColour& lightDiffuse,
  const AmjuGL::LightColour& lightSpecular,
  const AmjuGL::Vec3& lightPos)
{
  AMJU_CALL_STACK;

  float gAmbient[4] = { globalAmbient.m_r, globalAmbient.m_g, globalAmbient.m_b, 1.0f };
  float ambient[4] = { lightAmbient.m_r, lightAmbient.m_g, lightAmbient.m_b, 1.0f };
  float diffuse[4] = { lightDiffuse.m_r, lightDiffuse.m_g, lightDiffuse.m_b, 1.0f };
  float specular[4] = { lightSpecular.m_r, lightSpecular.m_g, lightSpecular.m_b, 1.0f };
  float pos[4] = { lightPos.m_x, lightPos.m_y, lightPos.m_z, 0 };

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glEnable(GL_LIGHT0);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gAmbient);
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
}
} // namespace Amju

#endif // IPHONE

