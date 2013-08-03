/*
Amju Games source code (c) Copyright Jason Colman 2010
*/

#ifdef AMJU_IOS

#include <AmjuFirst.h>
#include <math.h>
#include <stack>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
extern "C"
{
#include <GLKit/GLKMatrix4.h>
#include <GLKit/GLKMatrix3.h>
}
#include <AmjuGL.h>
#include <TriList.h>
#include "GLShader.h"
#include "AmjuGL-OpenGLES.2.h"
#include "ES2DefaultShaders.h"
#include <DegRad.h>
#include <ReportError.h>
#include <Texture.h>
#include <AmjuFinal.h>

////#define LANDSCAPE

namespace Amju
{
static GLKMatrix4 s_matrices[3]; // index with AmjuGL::MatrixMode
static AmjuGL::MatrixMode s_matrixMode;
  
// Matrix stacks
typedef std::stack<GLKMatrix4> MatrixStack;
MatrixStack m_stacks[3];
  
static Colour s_colour; // current colour - pass to shader
  
static GLShader* s_defaultShader = 0; // TODO TEMP TEST
  
// Remember the current texture type. If sphere mapped, no need to send
// texture coords to the graphics card.
// TODO Not very useful for ES - but can use to check that unsupported modes are not used
static AmjuGL::TextureType s_tt = AmjuGL::AMJU_TEXTURE_REGULAR;

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
  
    int vertexAttribPosition = glGetAttribLocation(s_defaultShader->GetProgHandle(), "position");
    int vertexAttribNormal = glGetAttribLocation(s_defaultShader->GetProgHandle(), "normal");
    int vertexAttribTexCoord0 = glGetAttribLocation(s_defaultShader->GetProgHandle(), "uv");
  
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


class TriListDynamicES2 : public TriListStatic
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
      
      int vertexAttribPosition = glGetAttribLocation(s_defaultShader->GetProgHandle(), "position");
      int vertexAttribNormal = glGetAttribLocation(s_defaultShader->GetProgHandle(), "normal");
      int vertexAttribTexCoord0 = glGetAttribLocation(s_defaultShader->GetProgHandle(), "uv");
      
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

AmjuGLOpenGLES2::AmjuGLOpenGLES2()
{
  s_factory.Add(TriListStatic::DRAWABLE_TYPE_ID, MakeStaticTriList);
  s_factory.Add(TriListDynamic::DRAWABLE_TYPE_ID, MakeDynamicTriList);
}

Drawable* AmjuGLOpenGLES2::Create(int drawableTypeId)
{
  return s_factory.Create(drawableTypeId);
}
  
void AmjuGLOpenGLES2::Init()
{
  AmjuGLOpenGLBase::Init();
  
  s_shaderSupport = true; // ES2 so shaders must be supported!
  
  // Create default shader
  s_defaultShader = new GLShader;
  std::string vertSrc = DEFAULT_VERT_SRC;
  std::string fragSrc = DEFAULT_FRAG_SRC;
  
  if (!s_defaultShader->Create(vertSrc, fragSrc))
  {
    ReportError("Failed to create default shader for ES2: " + s_defaultShader->GetErrorString());
    Assert(0);
    return;
  }

  // TODO Create in code so not dependent on assets
  s_defaultTex = new Texture;
  s_defaultTex->Load("wh8.png");
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
	
  // TODO Do this with a different shader
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
    glDisable(glFlag);
  }
}

void AmjuGLOpenGLES2::DrawTriList(const AmjuGL::Tris& tris)
{
  Assert(0); // Dude, use TriList
/*
 GLfloat gCubeVertexData[216] =
 {
   // Data layout for each line below is:
   // positionX, positionY, positionZ,     normalX, normalY, normalZ,
   0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,
   .....
 
*/
  
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
//  static GLKMatrix4 s_matrices[3]; // index with AmjuGL::MatrixMode
//  static AmjuGL::MatrixMode s_matrixMode;
  
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
}    

} // namespace Amju

#endif // AMJU_IOS

