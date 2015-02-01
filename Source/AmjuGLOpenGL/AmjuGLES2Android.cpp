/*
Amju Games source code (c) Copyright Jason Colman 2010
*/

// This implementation of AmjuGLOpenGLES2 doesn't use 
//  iOS or Android-specific APIs, so should work on both.
// Also should work on Windows or Mac with ES2 layer (PowerVR, Mali).
// Good for testing, but may not be most efficient.

#if defined(AMJU_USE_ES2) && !defined(AMJU_IOS)

#include <AmjuFirst.h>
#include <math.h>
#include <stack>
#include "Internal/OpenGL.h"
#include "Internal/MATRIX4X4.h"
#include <AmjuGL.h>
#include <TriList.h>
#include "GLShader.h"
#include "AmjuGL-OpenGLES.2.h"
#include "ES2DefaultShaders.h"
#include "ShadowMapES2.h"
#include "RenderToTextureES2.h"
#include "CubemapOpenGL.h"
#include <DegRad.h>
#include <ReportError.h>
#include <Texture.h>
#include <AmjuFinal.h>

#ifndef int32_t
typedef int int32_t;
#endif

namespace Amju
{
typedef MATRIX4X4 Mtx4;
static Mtx4 s_matrices[3];
typedef std::stack<Mtx4> MatrixStack;

static AmjuGL::MatrixMode s_matrixMode;
  
// Matrix stacks
MatrixStack m_stacks[3];
  
static Colour s_colour; // current colour - pass to shader
  
static GLShader* s_defaultShader;
static GLShader* s_currentShader;

static bool s_lightingEnabled = true;
  
// Remember the current texture type (UV or sphere mapped)
static AmjuGL::TextureType s_tt = AmjuGL::AMJU_TEXTURE_REGULAR;

// Mode: wrap or clamp 
static AmjuGL::TextureMode s_tm = AmjuGL::AMJU_TEXTURE_WRAP;

// White texture for when texturing is "disabled"
static Texture* s_defaultTex;

static DrawableFactory s_factory;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

  static void __gluMakeIdentityf(GLfloat m[16])
  {
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
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
  
static void SetUpShader()
{
    Mtx4& projectionMatrix = s_matrices[AmjuGL::AMJU_PROJECTION_MATRIX];
    Mtx4& modelViewMatrix = s_matrices[AmjuGL::AMJU_MODELVIEW_MATRIX];
  
    // Modelview * projection matrix for world transforms
    Mtx4 modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
  
    glActiveTexture(GL_TEXTURE0);
  
    s_currentShader->Begin();
    s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_MODELVIEWPROJECTION_MATRIX, modelViewProjectionMatrix.entries);
    if (s_lightingEnabled)
    {
      // Inverse transpose of modelview matrix to rotate normals
      float normalMatrix[9] = 
      {
        modelViewMatrix.entries[0], modelViewMatrix.entries[1], modelViewMatrix.entries[2],
        modelViewMatrix.entries[4], modelViewMatrix.entries[5], modelViewMatrix.entries[6],
        modelViewMatrix.entries[8], modelViewMatrix.entries[9], modelViewMatrix.entries[10],
      };
      s_currentShader->SetMatrix3x3(AMJU_ES2_DEFAULT_SHADER_NORMAL_MATRIX, normalMatrix);
    }
    s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_TEXTURE, (AmjuGL::TextureHandle)0);
    s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_COLOUR, s_colour);
    s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_USE_LIGHTING, (float)(s_lightingEnabled ? 0 : 1));
    s_currentShader->SetInt(AMJU_ES2_DEFAULT_SHADER_USE_SPHEREMAP, (int)s_tt);
}

/*
static void DrawTriList(const AmjuGL::Tris& tris)
{
  AMJU_CALL_STACK;

  int numTris = tris.size();

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

  // ??? TODO ???
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}
*/
  
class TriListStaticES2 : public TriListStatic
{
public:

  virtual void Draw()
  {
//    DrawTriList(m_tris);

    SetUpShader();

    int vertexAttribPosition = s_defaultShader->GetAttribLocation(AMJU_ES2_DEFAULT_SHADER_POSITION);
    int vertexAttribNormal = s_defaultShader->GetAttribLocation(AMJU_ES2_DEFAULT_SHADER_NORMAL);
    int vertexAttribTexCoord0 = s_defaultShader->GetAttribLocation(AMJU_ES2_DEFAULT_SHADER_UV);

    // Bind the VBO
    glBindBuffer( GL_ARRAY_BUFFER, vbo_ );

    int32_t iStride = sizeof(AmjuGL::Vert);
    // Pass the vertex data
    glVertexAttribPointer(vertexAttribPosition, 3, GL_FLOAT, GL_FALSE, iStride,
            BUFFER_OFFSET( 0 ) );
    glEnableVertexAttribArray(vertexAttribPosition);

    glVertexAttribPointer(vertexAttribNormal, 3, GL_FLOAT, GL_FALSE, iStride,
            BUFFER_OFFSET( 3 * sizeof(GLfloat) ) );
    glEnableVertexAttribArray(vertexAttribNormal);

    glVertexAttribPointer(vertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, iStride,
                          BUFFER_OFFSET( 6 * sizeof(GLfloat) ) );
    glEnableVertexAttribArray(vertexAttribTexCoord0);
    
    glDrawArrays(GL_TRIANGLES, 0, num_vertices_);
  }

  virtual void Set(const AmjuGL::Tris& tris)
  {
    // Adapted from Android NDK Teapots sample

    //Create VBO
    num_vertices_ = tris.size() * 3;
    int32_t iStride = sizeof(AmjuGL::Vert);
/*
    int32_t iIndex = 0;
    TEAPOT_VERTEX* p = new TEAPOT_VERTEX[num_vertices_];
    for( int32_t i = 0; i < num_vertices_; ++i )
    {
        p[i].pos[0] = teapotPositions[iIndex];
        p[i].pos[1] = teapotPositions[iIndex + 1];
        p[i].pos[2] = teapotPositions[iIndex + 2];

        p[i].normal[0] = teapotNormals[iIndex];
        p[i].normal[1] = teapotNormals[iIndex + 1];
        p[i].normal[2] = teapotNormals[iIndex + 2];
        iIndex += 3;
    }
*/
    glGenBuffers( 1, &vbo_ );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
    glBufferData( GL_ARRAY_BUFFER, iStride * num_vertices_, &(tris[0]), GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
  }

  virtual bool Init() { return true; }

private:
  ////AmjuGL::Tris m_tris;
  GLuint vbo_; // TODO
  int num_vertices_; // TODO
};


class TriListDynamicES2 : public TriListDynamic
{
public:
  
  virtual void Draw()
  {
  }

  virtual void Set(const AmjuGL::Tris& tris)
  {
  }

  virtual bool Init() { return true; }

private:
};

static Drawable* MakeStaticTriList()
{
  return new TriListStaticES2;
}

static Drawable* MakeDynamicTriList()
{
  return new TriListDynamicES2;
}

static Drawable* MakeShadowMapES2()
{
	return new ShadowMapES2;
}

static Drawable* MakeRenderToTextureES2()
{
	return new RenderToTextureES2;
}

static Drawable* MakeCubemap()
{
	return new CubemapOpenGL;
}

AmjuGLOpenGLES2::AmjuGLOpenGLES2()
{
  s_factory.Add(TriListStatic::DRAWABLE_TYPE_ID, MakeStaticTriList);
  s_factory.Add(TriListDynamic::DRAWABLE_TYPE_ID, MakeDynamicTriList);
  s_factory.Add(ShadowMap::DRAWABLE_TYPE_ID, MakeShadowMapES2);
  s_factory.Add(RenderToTexture::DRAWABLE_TYPE_ID, MakeRenderToTextureES2);
  s_factory.Add(Cubemap::DRAWABLE_TYPE_ID, MakeCubemap);
}

Drawable* AmjuGLOpenGLES2::Create(int drawableTypeId)
{
  return s_factory.Create(drawableTypeId);
}
  
void AmjuGLOpenGLES2::Init()
{
  AmjuGLOpenGLBase::Init();
  
  s_shaderSupport = true; // ES2 so shaders must be supported!
  
  // Create shaders
  s_defaultShader = new GLShader;
  if (!s_defaultShader->Create(DEFAULT_VERT_SRC, DEFAULT_FRAG_SRC))
  {
    ReportError("Failed to create default shader for ES2: " + s_defaultShader->GetErrorString());
    Assert(0);
    return;
  }

  s_currentShader = s_defaultShader;
  s_lightingEnabled = true;
  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
//  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  
  // Create in code so not dependent on assets
  s_defaultTex = new Texture;
  unsigned char data[] =  { 0xff, 0xff, 0xff };
  s_defaultTex->Create(data, 1, 1, 3); // w, h, bytes per pixel
}

bool AmjuGLOpenGLES2::CreateWindow(AmjuGLWindowInfo*)
{
  return true;
}
	
void AmjuGLOpenGLES2::Flip()
{
}	

void AmjuGLOpenGLES2::UseShader(Shader* sh)
{
    if (sh)
    {
        s_currentShader = dynamic_cast<GLShader*>(sh);
    }
    else
    {
        s_currentShader = s_defaultShader;
    }
    s_currentShader->UseThisShader();
}    
    
Shader* AmjuGLOpenGLES2::LoadShader(const std::string& shaderFileName)
{
  AMJU_CALL_STACK;

  GLShader* s = new GLShader;
  // TODO Add "ogl" to shader file name
  // TODO Two separate files for frag and vertex
  if (!s->Load(shaderFileName))
  {
    delete s;
    return nullptr;
  }
  return s;
}
	
void AmjuGLOpenGLES2::SetTextureMode(AmjuGL::TextureMode tm)
{
  AMJU_CALL_STACK;
  s_tm = tm;
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
	
	int wrapmode = GL_REPEAT;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmode);
		
	// Mipmapping/filtering: these settings are recommended.
	// Use this - but requires mipmaps to be created ?
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	// Allowed values for min filter:
	// GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST,
	// GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
	
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
  
  s_tt = tt;
}

static uint32 ConvertToGLFlag(uint32 flag)
{
  AMJU_CALL_STACK;

  switch (flag)
  {
  case AmjuGL::AMJU_BLEND:
    return GL_BLEND;
  case AmjuGL::AMJU_DEPTH_READ:
    return GL_DEPTH_TEST;
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
  else if (flag == AmjuGL::AMJU_LIGHTING)
  {
    // Set lighting flag in shader
    s_lightingEnabled = true;
    return;
  }
  else if (flag == AmjuGL::AMJU_TEXTURE_2D)
  {
    // Do nothing, right?
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
  else if (flag == AmjuGL::AMJU_LIGHTING)
  {
    // Set lighting flag in shader
    s_lightingEnabled = false;
    return;
  }
  else if (flag == AmjuGL::AMJU_TEXTURE_2D)
  {
    // Use white texture
    s_defaultTex->UseThisTexture();
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
  Assert(0); // Dude, use TriList
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
  s_colour = Colour(r, g, b, a);
}


void AmjuGLOpenGLES2::GetMatrix(AmjuGL::MatrixMode m, float result[16])
{
  AMJU_CALL_STACK;  
  Mtx4& mat = s_matrices[m];
  for (int i = 0; i < 16; i++)
  {
    result[i] = mat.entries[i];
  }
}

void AmjuGLOpenGLES2::SetMatrixMode(AmjuGL::MatrixMode m)
{
  AMJU_CALL_STACK;
  s_matrixMode = m;
}

void AmjuGLOpenGLES2::SetIdentity()
{
  AMJU_CALL_STACK;
  s_matrices[s_matrixMode].LoadIdentity();
}

void AmjuGLOpenGLES2::PushMatrix()
{
  AMJU_CALL_STACK;
  m_stacks[s_matrixMode].push(s_matrices[s_matrixMode]);
}

void AmjuGLOpenGLES2::PopMatrix()
{
  AMJU_CALL_STACK;
  
  MatrixStack& ms = m_stacks[s_matrixMode];
  Assert(!ms.empty());
  Mtx4 mat = ms.top();
  ms.pop();
  s_matrices[s_matrixMode] = mat;
}

void AmjuGLOpenGLES2::Translate(float x, float y, float z)
{
  AMJU_CALL_STACK;
  
  Mtx4 mat;
  mat.SetTranslation(VECTOR3D(x, y, z));
  MultMatrix(mat.entries);
}

void AmjuGLOpenGLES2::Scale(float x, float y, float z)
{
  AMJU_CALL_STACK;

  Mtx4 mat;
  mat.SetScale(VECTOR3D(x, y, z));
  MultMatrix(mat.entries);
}

void AmjuGLOpenGLES2::RotateX(float degs)
{
  AMJU_CALL_STACK;

  Mtx4 mat;
  mat.SetRotationX(degs);
  MultMatrix(mat.entries);
}

void AmjuGLOpenGLES2::RotateY(float degs)
{
  AMJU_CALL_STACK;

  Mtx4 mat;
  mat.SetRotationY(degs);
  MultMatrix(mat.entries);
}

void AmjuGLOpenGLES2::RotateZ(float degs)
{
  AMJU_CALL_STACK;

  Mtx4 mat;
  mat.SetRotationZ(degs);
  MultMatrix(mat.entries);
}

void AmjuGLOpenGLES2::SetPerspectiveProjection(float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;
    
  // Expect current matrix mode to be projection
  Assert(AmjuGL::GetMatrixMode() == AmjuGL::AMJU_PROJECTION_MATRIX);
    
  Mtx4 persp;
  persp.SetPerspective(fov, aspectRatio, nearDist, farDist);
  MultMatrix(persp);
////  s_matrices[AmjuGL::AMJU_PROJECTION_MATRIX] = persp * s_matrices[AmjuGL::AMJU_PROJECTION_MATRIX];
}
  
void AmjuGLOpenGLES2::LookAt(
  float eyex, float eyey, float eyez,
  float centerx, float centery, float centerz,
  float upx, float upy, float upz)
{
  AMJU_CALL_STACK;
    
  // Expect current matrix mode to be mview
  Assert(AmjuGL::GetMatrixMode() == AmjuGL::AMJU_MODELVIEW_MATRIX);
  
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
  
  MultMatrix(&m[0][0]);
  Translate(-eyex, -eyey, -eyez);
}
  
  
void AmjuGLOpenGLES2::MultMatrix(const float array[16])
{
  AMJU_CALL_STACK;
  Mtx4 mat((float*)array);
  s_matrices[s_matrixMode] = s_matrices[s_matrixMode] * mat;
}
    
void AmjuGLOpenGLES2::DrawLighting(
  const AmjuGL::LightColour& globalAmbient,
  const AmjuGL::LightColour& lightAmbient,
  const AmjuGL::LightColour& lightDiffuse,
  const AmjuGL::LightColour& lightSpecular,
  const AmjuGL::Vec3& lightPos)
{
  AMJU_CALL_STACK;
  // TODO Set uniform vars for shader
  
}  
  
void AmjuGLOpenGLES2::SetOrthoProjection()
{
  // TODO Get rid
}

} // namespace Amju

#endif // ANDROID_NDK

