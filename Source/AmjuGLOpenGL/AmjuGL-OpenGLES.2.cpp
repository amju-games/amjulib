/*
Amju Games source code (c) Copyright Jason Colman 2010
*/

#if defined(AMJU_IOS) 

#include <AmjuFirst.h>
#include <math.h>
#include <stack>
#include "Internal/OpenGL.h"
#ifdef AMJU_IOS
extern "C"
{
#include <GLKit/GLKMatrix4.h>
#include <GLKit/GLKMatrix3.h>
}
#endif

#include <AmjuGL.h>
#include <TriList.h>
#include <ShaderNull.h>
#include "GLShader.h"
#include "AmjuGL-OpenGLES.2.h"
#include "ES2DefaultShaders.h"
#include "ShadowMapES2.h"
#include "RenderToTextureES2.h"
//#include "RenderToTextureOpenGL.h"
#include "ShadowMapOpenGL.h"
#include <DegRad.h>
#include <ReportError.h>
#include <Texture.h>
#include <AmjuFinal.h>

namespace Amju
{
static GLKMatrix4 s_matrices[3]; // index with AmjuGL::MatrixMode
typedef std::stack<GLKMatrix4> MatrixStack;

static AmjuGL::MatrixMode s_matrixMode;
  
// Matrix stacks
MatrixStack m_stacks[3];
  
static Colour s_colour; // current colour - pass to shader
  
static GLShader* s_defaultShader;
static GLShader* s_currentShader;

static bool s_lightingEnabled = true;
static AmjuGL::Vec3 s_lightPos(0, 0, 1);
  
// Remember the current texture type. If sphere mapped, no need to send
// texture coords to the graphics card.
// TODO Not very useful for ES - but can use to check that unsupported modes are not used
static AmjuGL::TextureType s_tt = AmjuGL::AMJU_TEXTURE_REGULAR;
static AmjuGL::TextureMode s_tm = AmjuGL::AMJU_TEXTURE_WRAP;

// White texture for when texturing is "disabled"
static Texture* s_defaultTex;

static DrawableFactory s_factory;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
    
class TriListStaticES2 : public TriListStatic
{
public:
  TriListStaticES2()
  {
    m_numVerts = 0;
    glGenVertexArraysOES(1, &m_vertexArray);
    glGenBuffers(1, &m_vertexBuffer);    
  }
 
  ~TriListStaticES2()
  {
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteVertexArraysOES(1, &m_vertexArray);
  }

  virtual void Draw()
  {
    glActiveTexture(GL_TEXTURE0);
  
    s_currentShader->Begin();
    if (s_currentShader == s_defaultShader)
    {
      // TODO only change these when necessary.
      // TODO Check for using default shader or a different one
      GLKMatrix4& projectionMatrix = s_matrices[AmjuGL::AMJU_PROJECTION_MATRIX];
      GLKMatrix4& modelViewMatrix = s_matrices[AmjuGL::AMJU_MODELVIEW_MATRIX];
      
      // Moldeview * projection matrix for world transforms
      GLKMatrix4 modelViewProjectionMatrix = GLKMatrix4Multiply(projectionMatrix, modelViewMatrix);
      
      s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_MODELVIEWPROJECTION_MATRIX, modelViewProjectionMatrix.m);
      if (s_lightingEnabled)
      {
        // Inverse transpose of modelview matrix to rotate normals
        GLKMatrix3 normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(modelViewMatrix), NULL);
        s_currentShader->SetMatrix3x3(AMJU_ES2_DEFAULT_SHADER_NORMAL_MATRIX, normalMatrix.m);
      }
      s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_TEXTURE, (AmjuGL::TextureHandle)0);
      s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_COLOUR, s_colour);
      s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_USE_LIGHTING, (float)(s_lightingEnabled ? 0 : 1));
      s_currentShader->SetInt(AMJU_ES2_DEFAULT_SHADER_USE_SPHEREMAP, (int)s_tt);
      s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_LIGHT_DIR, s_lightPos);
    }
    glBindVertexArrayOES(m_vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, m_numVerts);
  }

  virtual void Set(const AmjuGL::Tris& tris)
  {
    m_numVerts = tris.size() * 3;

    glBindVertexArrayOES(m_vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(AmjuGL::Vert) * m_numVerts, &(tris[0].m_verts[0].m_x), GL_STATIC_DRAW);

    // This should probably go in Draw
    const int STRIDE = sizeof(AmjuGL::Vert);
  
    // Use lighting shader so we get the Normal attrib variable location!
    int vertexAttribPosition = s_currentShader->GetAttribLocation(AMJU_ES2_DEFAULT_SHADER_POSITION);
    int vertexAttribNormal = s_currentShader->GetAttribLocation(AMJU_ES2_DEFAULT_SHADER_NORMAL);
    int vertexAttribTexCoord0 = s_currentShader->GetAttribLocation(AMJU_ES2_DEFAULT_SHADER_UV);
  
    glEnableVertexAttribArray(vertexAttribPosition);
    glVertexAttribPointer(vertexAttribPosition, 3, GL_FLOAT, GL_FALSE, STRIDE, BUFFER_OFFSET(0));
  
    glEnableVertexAttribArray(vertexAttribNormal);
    glVertexAttribPointer(vertexAttribNormal, 3, GL_FLOAT, GL_FALSE, STRIDE, BUFFER_OFFSET(12));
  
    glEnableVertexAttribArray(vertexAttribTexCoord0);
    glVertexAttribPointer(vertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, STRIDE, BUFFER_OFFSET(24));

    glBindVertexArrayOES(0);
  }

  virtual bool Init() { return true; }

private:
  GLuint m_vertexArray;
  GLuint m_vertexBuffer;
  int m_numVerts;
};


class TriListDynamicES2 : public TriListDynamic
{
public:
  TriListDynamicES2()
  {
    m_firstSet = true;
    m_numVerts = 0;
    glGenVertexArraysOES(1, &m_vertexArray);
    glGenBuffers(1, &m_vertexBuffer);
  }
  
  ~TriListDynamicES2()
  {
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteVertexArraysOES(1, &m_vertexArray);
  }
  
  virtual void Draw()
  {
    // TODO Factor out common code!!
    
    glActiveTexture(GL_TEXTURE0);
    
    s_currentShader->Begin();
    if (s_currentShader == s_defaultShader)
    {
      // TODO only change these when necessary.
      // TODO Check for using default shader or a different one
      GLKMatrix4& projectionMatrix = s_matrices[AmjuGL::AMJU_PROJECTION_MATRIX];
      GLKMatrix4& modelViewMatrix = s_matrices[AmjuGL::AMJU_MODELVIEW_MATRIX];
      
      // Modelview * projection matrix for world transforms
      GLKMatrix4 modelViewProjectionMatrix = GLKMatrix4Multiply(projectionMatrix, modelViewMatrix);
      
      s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_MODELVIEWPROJECTION_MATRIX, modelViewProjectionMatrix.m);
      if (s_lightingEnabled)
      {
        // Inverse transpose of modelview matrix to rotate normals
        GLKMatrix3 normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(modelViewMatrix), NULL);
        s_currentShader->SetMatrix3x3(AMJU_ES2_DEFAULT_SHADER_NORMAL_MATRIX, normalMatrix.m);
      }
      s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_TEXTURE, (AmjuGL::TextureHandle)0);
      s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_COLOUR, s_colour);
      s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_USE_LIGHTING, (float)(s_lightingEnabled ? 0 : 1));
      s_currentShader->SetInt(AMJU_ES2_DEFAULT_SHADER_USE_SPHEREMAP, (int)s_tt);
      s_currentShader->Set(AMJU_ES2_DEFAULT_SHADER_LIGHT_DIR, s_lightPos);
    }
    
    glBindVertexArrayOES(m_vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, m_numVerts);
  }

  virtual void Set(const AmjuGL::Tris& tris)
  {
    if (m_firstSet)
    {
      m_firstSet = false;
      
      m_numVerts = tris.size() * 3;
      
      glBindVertexArrayOES(m_vertexArray);
      
      glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(AmjuGL::Vert) * m_numVerts, &(tris[0].m_verts[0].m_x), GL_DYNAMIC_DRAW);
      
      // This should probably go in Draw
      const int STRIDE = sizeof(AmjuGL::Vert);
      
      int vertexAttribPosition = s_defaultShader->GetAttribLocation(AMJU_ES2_DEFAULT_SHADER_POSITION);
      int vertexAttribNormal = s_defaultShader->GetAttribLocation(AMJU_ES2_DEFAULT_SHADER_NORMAL);
      int vertexAttribTexCoord0 = s_defaultShader->GetAttribLocation(AMJU_ES2_DEFAULT_SHADER_UV);
      
      glEnableVertexAttribArray(vertexAttribPosition);
      glVertexAttribPointer(vertexAttribPosition, 3, GL_FLOAT, GL_FALSE, STRIDE, BUFFER_OFFSET(0));
      
      glEnableVertexAttribArray(vertexAttribNormal);
      glVertexAttribPointer(vertexAttribNormal, 3, GL_FLOAT, GL_FALSE, STRIDE, BUFFER_OFFSET(12));
      
      glEnableVertexAttribArray(vertexAttribTexCoord0);
      glVertexAttribPointer(vertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, STRIDE, BUFFER_OFFSET(24));
      
      glBindVertexArrayOES(0);
      
    }
    else
    {
      // Must have same number of tris, right?
      Assert(tris.size() * 3 == m_numVerts);
      
      glBindVertexArrayOES(m_vertexArray);
      
      glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(AmjuGL::Vert) * m_numVerts, &(tris[0].m_verts[0].m_x));
    }
  }

  virtual bool Init() { return true; }

private:
  GLuint m_vertexArray;
  GLuint m_vertexBuffer;
  int m_numVerts;
  bool m_firstSet;
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
  return new ShadowMapOpenGL;
}

static Drawable* MakeRenderToTextureES2()
{
  return new RenderToTextureES2;
}

AmjuGLOpenGLES2::AmjuGLOpenGLES2()
{
  s_factory.Add(TriListStatic::DRAWABLE_TYPE_ID, MakeStaticTriList);
  s_factory.Add(TriListDynamic::DRAWABLE_TYPE_ID, MakeDynamicTriList);
  s_factory.Add(ShadowMap::DRAWABLE_TYPE_ID, MakeShadowMapES2);
  s_factory.Add(RenderToTexture::DRAWABLE_TYPE_ID, MakeRenderToTextureES2);
}

Drawable* AmjuGLOpenGLES2::Create(int drawableTypeId)
{
  return s_factory.Create(drawableTypeId);
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

Shader* AmjuGLOpenGLES2::LoadShader(const std::string& shaderFileName)
{
  AMJU_CALL_STACK;

  GLShader* s = new GLShader;
  // TODO Add "ogl" to shader file name
  // TODO Two separate files for frag and vertex
  if (!s->Load(shaderFileName))
  {
    std::cout << "FAILED TO LOAD SHADER!! " << shaderFileName << "\n";
    Assert(0);
    
    delete s;
    return nullptr; // new ShaderNull;
  }
  return s;
}
	
void AmjuGLOpenGLES2::SetPerspectiveProjection(float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;

  // Expect current matrix mode to be perspective
  s_matrices[AmjuGL::AMJU_PROJECTION_MATRIX] = GLKMatrix4Multiply(
    s_matrices[AmjuGL::AMJU_PROJECTION_MATRIX],
    GLKMatrix4MakePerspective(DegToRad(fov), aspectRatio, nearDist, farDist));
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
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmode);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmode);
		
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

  AmjuGL::Vert v[2] =
  {
    AmjuGL::Vert(v1.m_x, v1.m_y, v1.m_z, 0, 0,  0, 1, 0),
    AmjuGL::Vert(v2.m_x, v2.m_y, v2.m_z, 0, 0,  0, 1, 0)
  };


  // TODO!
//  glDrawArrays(GL_LINES, 0, 2);

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
/*
  AMJU_CALL_STACK;
  
  // TODO only change these when necessary.
  // TODO Check for using default shader or a different one
  GLKMatrix4& projectionMatrix = s_matrices[AmjuGL::AMJU_PROJECTION_MATRIX];
  GLKMatrix4& modelViewMatrix = s_matrices[AmjuGL::AMJU_MODELVIEW_MATRIX];
  
  // Inverse transpose of modelview matrix to rotate normals
  GLKMatrix3 _normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(modelViewMatrix), NULL);
  // Moldeview * projection matrix for world transforms
  GLKMatrix4 _modelViewProjectionMatrix = GLKMatrix4Multiply(projectionMatrix, modelViewMatrix);
  
  glActiveTexture(GL_TEXTURE0);
  
  s_defaultShader->Begin();
  s_defaultShader->Set(AMJU_ES2_DEFAULT_SHADER_MODELVIEWPROJECTION_MATRIX, _modelViewProjectionMatrix.m);
  s_defaultShader->Set("Texture", (AmjuGL::TextureHandle)0); // glUniform1i(_textureUniform, 0);
  s_defaultShader->Set("colour", s_colour);
  
  // TODO Set 3*3 normals matrix
  
  // TODO -  need to make this efficient. Only create new buffers when necessary.
  int numVerts = tris.size() * 3;
  
  GLuint _vertexArray;
  GLuint _vertexBuffer;

  glGenVertexArraysOES(1, &_vertexArray);
  glBindVertexArrayOES(_vertexArray);
  
  glGenBuffers(1, &_vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(AmjuGL::Vert) * numVerts, &(tris[0].m_verts[0].m_x), GL_STATIC_DRAW);
  
  const int STRIDE = sizeof(AmjuGL::Vert);
  
  int GLKVertexAttribPosition = glGetAttribLocation(s_defaultShader->GetProgHandle(), "position");
  int GLKVertexAttribNormal = glGetAttribLocation(s_defaultShader->GetProgHandle(), "normal");
  int GLKVertexAttribTexCoord0 = glGetAttribLocation(s_defaultShader->GetProgHandle(), "uv");
  
  glEnableVertexAttribArray(GLKVertexAttribPosition);
  glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, STRIDE, BUFFER_OFFSET(0));
  
  glEnableVertexAttribArray(GLKVertexAttribNormal);
  glVertexAttribPointer(GLKVertexAttribNormal, 3, GL_FLOAT, GL_FALSE, STRIDE, BUFFER_OFFSET(12));
  
  glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
  glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, STRIDE, BUFFER_OFFSET(24));
  
  glBindVertexArrayOES(_vertexArray);
  
  glDrawArrays(GL_TRIANGLES, 0, numVerts);
  
  glDeleteBuffers(1, &_vertexBuffer);
  glDeleteVertexArraysOES(1, &_vertexArray);
 
  s_defaultShader->End(); // Currently not requried??
*/
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
  GLKMatrix4& mat = s_matrices[m];
  for (int i = 0; i < 16; i++)
  {
    result[i] = mat.m[i];
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
  s_matrices[s_matrixMode] = GLKMatrix4Identity;
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
  GLKMatrix4 mat = ms.top();
  ms.pop();
  s_matrices[s_matrixMode] = mat;
}

void AmjuGLOpenGLES2::Translate(float x, float y, float z)
{
  AMJU_CALL_STACK;
  
  GLKMatrix4 mat = GLKMatrix4MakeTranslation(x, y, z);
  s_matrices[s_matrixMode] = GLKMatrix4Multiply(s_matrices[s_matrixMode], mat);
}

void AmjuGLOpenGLES2::Scale(float x, float y, float z)
{
  AMJU_CALL_STACK;

  GLKMatrix4 mat = GLKMatrix4MakeScale(x, y, z);
  s_matrices[s_matrixMode] = GLKMatrix4Multiply(s_matrices[s_matrixMode], mat);
}

void AmjuGLOpenGLES2::RotateX(float degs)
{
  AMJU_CALL_STACK;

  s_matrices[s_matrixMode] = GLKMatrix4RotateX(s_matrices[s_matrixMode], DegToRad(degs));
}

void AmjuGLOpenGLES2::RotateY(float degs)
{
  AMJU_CALL_STACK;
  s_matrices[s_matrixMode] = GLKMatrix4RotateY(s_matrices[s_matrixMode], DegToRad(degs));
}

void AmjuGLOpenGLES2::RotateZ(float degs)
{
  AMJU_CALL_STACK;
  s_matrices[s_matrixMode] = GLKMatrix4RotateZ(s_matrices[s_matrixMode], DegToRad(degs));
}

void AmjuGLOpenGLES2::MultMatrix(const float array[16])
{
  AMJU_CALL_STACK;
  GLKMatrix4 mat = GLKMatrix4MakeWithArray((float*)array);
  s_matrices[s_matrixMode] = GLKMatrix4Multiply(s_matrices[s_matrixMode], mat); 
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
  s_lightPos = lightPos;
}    

} // namespace Amju

#endif // AMJU_IOS

