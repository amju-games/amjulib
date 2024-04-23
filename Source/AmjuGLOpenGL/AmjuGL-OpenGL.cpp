/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#include <AmjuFirst.h>
#if (defined (WIN32) || defined (MACOSX)) && !defined(AMJU_USE_ES2)

#ifdef WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif

#include <iostream>
//#include "GL/glew.h"
#include <TriList.h>
#include "AmjuGL-OpenGL.h"
#include <CalcTangents.h>
#include "GLShader.h"
#include "ShaderNull.h"
#include "AmjuAssert.h"
#include "Internal/OpenGL.h"
#include "ShadowMapOpenGL1.h"
#include "ShadowMapOpenGL2.h"
#include "ShadowMapOpenGL3.h"
#include "ShadowMapOpenGL.h"
#include "RenderToTextureOpenGL.h"
#include "CubemapOpenGL.h"
#include <Matrix.h> // TODO TEMP TEST
#include <AmjuFinal.h>

#define SHADER_DEBUG
//#define OPENGL_SHOW_INFO

#undef CreateWindow

namespace Amju
{
static GLShader* s_shader = nullptr;
static Colour s_openGlColour; // current colour - pass to shader

static DrawableFactory s_factory;

class TriListStaticOpenGL : public TriListStatic
{
public:
  virtual void Draw()
  {
    if (!m_tris.empty())
    {
      AmjuGL::DrawTriList(m_tris);
    }
  }

  virtual void Set(const AmjuGL::Tris& tris)
  {
    m_tris = tris;
  }
  
  virtual bool Init() { return true; }

  void CalcTangents() override
  {
    Amju::CalcTangents(m_tris);
  }

private:
  AmjuGL::Tris m_tris;
};

class TriListDynamicOpenGL : public TriListDynamic
{
public:
  virtual void Draw()
  {
    if (!m_tris.empty())
    {
      AmjuGL::DrawTriList(m_tris);
    }
  }

  virtual void Set(const AmjuGL::Tris& tris)
  {
    m_tris = tris;
  }
  
  virtual bool Init() { return true; }

  void CalcTangents() override
  {
    Amju::CalcTangents(m_tris);
  }

private:
  AmjuGL::Tris m_tris;
};

static Drawable* MakeStaticTriList()
{
  return new TriListStaticOpenGL;
}

static Drawable* MakeDynamicTriList()
{
  return new TriListDynamicOpenGL;
}

static Drawable* MakeRenderToTexture()
{
std::cout << "Creating RenderToTexture OpenGL\n";

  return new RenderToTextureOpenGL;
}

static Drawable* MakeShadowMap()
{
  return new ShadowMapOpenGL;

#ifdef WIN32
  // TODO: create best quality impl depending on hardware capability
  if (!glBindFramebufferEXT) // check fn exists
  {
    // Required extension not supported
    std::cout << "AmjuGL: Open GL Shadow map: not supported.\n";
    return new ShadowMapNull;
  }
#endif

#ifdef USE_SHADOW_MAP_OPENGL_2
  std::cout << "Creating ShadowMapOpenGL2\n";
  return new ShadowMapOpenGL2;
#endif

  return new ShadowMapNull;
}

static Drawable* MakeCubemap()
{
  return new CubemapOpenGL;
}

// Remember the current texture type. If sphere mapped, no need to send
// texture coords to the graphics card.
static AmjuGL::TextureType s_tt = AmjuGL::AMJU_TEXTURE_REGULAR;

AmjuGLOpenGL::WindowCreateFunc windowCreateFunc = 0;

AmjuGLOpenGL::AmjuGLOpenGL(AmjuGLOpenGL::WindowCreateFunc f)
{
  windowCreateFunc = f;

  s_factory.Add(TriListStatic::DRAWABLE_TYPE_ID, MakeStaticTriList);
  s_factory.Add(TriListDynamic::DRAWABLE_TYPE_ID, MakeDynamicTriList);
  s_factory.Add(ShadowMap::DRAWABLE_TYPE_ID, MakeShadowMap);
  s_factory.Add(RenderToTexture::DRAWABLE_TYPE_ID, MakeRenderToTexture);
  s_factory.Add(Cubemap::DRAWABLE_TYPE_ID, MakeCubemap);
}

Drawable* AmjuGLOpenGL::Create(int drawableTypeId)
{
  return s_factory.Create(drawableTypeId);
}

bool AmjuGLOpenGL::CreateWindow(AmjuGLWindowInfo* wi)
{
  return windowCreateFunc(wi);
}

void AmjuGLOpenGL::Flip()
{
  AMJU_CALL_STACK;

#ifdef WIN32
  SwapBuffers(wglGetCurrentDC());
#endif
}

void AmjuGLOpenGL::SetColour(float r, float g, float b, float a)
{
  AMJU_CALL_STACK;

  GL_CHECK(glColor4f(r, g, b, a));
  s_openGlColour = Colour(r, g, b, a);
}

void AmjuGLOpenGL::DrawLighting(
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

  GL_CHECK(glLightfv(GL_LIGHT0, GL_AMBIENT, ambient));
  GL_CHECK(glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse));
  GL_CHECK(glLightfv(GL_LIGHT0, GL_SPECULAR, specular));
  GL_CHECK(glEnable(GL_LIGHT0));
  GL_CHECK(glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gAmbient));
  GL_CHECK(glLightfv(GL_LIGHT0, GL_POSITION, pos));
}

void AmjuGLOpenGL::Init()
{
  AMJU_CALL_STACK;

  AmjuGLOpenGLBase::Init();

  GL_CHECK(glEnable(GL_COLOR_MATERIAL)); 

#ifdef MACOSX
  s_shaderSupport = true;
#endif

#ifdef WIN32
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    // failed to initialize GLEW!
    std::cout << "Failed to initialise GLEW!\n";
  }

#ifdef SHADER_DEBUG
  std::cout << "Using GLEW Version: " << glewGetString(GLEW_VERSION) << "\n";
#endif

  // Check for OpenGL Shading Language
  if (glewIsSupported("GL_ARB_shading_language_100")) 
  {  
    int major, minor, revision;
    const GLubyte* sVersion = glGetString(GL_SHADING_LANGUAGE_VERSION_ARB);
    if (glGetError() == GL_INVALID_ENUM)
    {
      major = 1; minor = 0; revision=51;
#ifdef SHADER_DEBUG
std::cout << "GLSL v.1.0.51 is supported.\n";
#endif
    }
    else
    {
      // parse string sVersion to get major, minor, revision
#ifdef SHADER_DEBUG
std::cout << "GLSL " << sVersion << " is supported.\n";
#endif
    }
    s_shaderSupport = true;
  }
  else
  {
#ifdef SHADER_DEBUG
std::cout << "GLSL is NOT SUPPORTED :-(\n";
#endif
  }
#endif
}

void AmjuGLOpenGL::SetPerspectiveProjection(
  float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;

  gluPerspective(fov, aspectRatio, nearDist, farDist);
}

void AmjuGLOpenGL::SetOrthoProjection(
  float left, float right, float top, float bottom, float nearDist, float farDist)
{
  AMJU_CALL_STACK;

  GL_CHECK(glOrtho(left, right, bottom, top, nearDist, farDist));
}

void AmjuGLOpenGL::LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ)
{
  AMJU_CALL_STACK;

  gluLookAt(eyeX, eyeY, eyeZ, // origin - player coords
            x, y, z, // point in direction we want to look
            upX, upY, upZ /* 'Up' vector */);
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
  case AmjuGL::AMJU_CULLING:
    return GL_CULL_FACE;
  }
  return 0;
}

void AmjuGLOpenGL::Enable(uint32 flag)
{
  AMJU_CALL_STACK;

  if (flag == AmjuGL::AMJU_DEPTH_WRITE)
  {
    GL_CHECK(glDepthMask(GL_TRUE));
    return;
  }

  uint32 glFlag = ConvertToGLFlag(flag);
  if (glFlag)
  {
    GL_CHECK(glEnable(glFlag));
  }
}

void AmjuGLOpenGL::Disable(uint32 flag)
{
  AMJU_CALL_STACK;

  if (flag == AmjuGL::AMJU_DEPTH_WRITE)
  {
    GL_CHECK(glDepthMask(GL_FALSE));
    return;
  }

  uint32 glFlag = ConvertToGLFlag(flag);
  if (glFlag)
  {
    GL_CHECK(glDisable(glFlag));
  }
}

void AmjuGLOpenGL::DrawTriList(const AmjuGL::Tris& tris)
{
  AMJU_CALL_STACK;
  int numTris = tris.size();

  if (s_shader)
  {
    // Try to use attribute variables

    // TODO TEMP TEST
    int uMatrix = s_shader->FindUniformLocation("modelViewProjectionMatrix");
    if (uMatrix > -1)
    {
      Matrix mv;
      mv.ModelView();
      Matrix proj;
      proj.Projection();
      Matrix mat = mv * proj;
      s_shader->Set("modelViewProjectionMatrix", mat);
   
      int uNormalMatrix = s_shader->FindUniformLocation("normalMatrix");
      if (uNormalMatrix > -1)
      {
        // Inverse transpose of modelview matrix to rotate normals
        // TODO
        float normalMatrix[9] = 
        {
          mat[0], mat[1], mat[2],
          mat[4], mat[5], mat[6],
          mat[8], mat[9], mat[10],
        };
        s_shader->SetMatrix3x3("normalMatrix", normalMatrix);
      }
    }

    int uColour = s_shader->FindUniformLocation("colourUniform");
    if (uColour > -1)
    {
      s_shader->Set("colourUniform", s_openGlColour);
    }

    // Setup
    int aPosition = s_shader->FindAttribLocation("position");
    int aNormal   = s_shader->FindAttribLocation("normal");
    int aUV   = s_shader->FindAttribLocation("uv");
    int tan = s_shader->FindAttribLocation("tangent");
    int colour = s_shader->FindAttribLocation("colour");

    if (aPosition > -1)
    {
        GL_CHECK(glEnableVertexAttribArray(aPosition));
        GL_CHECK(glVertexAttribPointer(aPosition, 3, GL_FLOAT, false, sizeof(AmjuGL::Vert), &tris[0].m_verts[0].m_x));
    }

    if (aNormal > -1)
    {
        GL_CHECK(glEnableVertexAttribArray(aNormal));
        GL_CHECK(glVertexAttribPointer(aNormal, 3, GL_FLOAT, false, sizeof(AmjuGL::Vert), &tris[0].m_verts[0].m_nx));
    }

    if (aUV > -1)
    {
        GL_CHECK(glEnableVertexAttribArray(aUV));
        GL_CHECK(glVertexAttribPointer(aUV, 2, GL_FLOAT, false, sizeof(AmjuGL::Vert), &tris[0].m_verts[0].m_u));
    }

    if (tan > -1)
    {
        GL_CHECK(glEnableVertexAttribArray(tan));
        GL_CHECK(glVertexAttribPointer(tan, 3, GL_FLOAT, false, sizeof(AmjuGL::Vert), &tris[0].m_verts[0].m_tanx));
    }

    if (colour > -1)
    {
      GL_CHECK(glEnableVertexAttribArray(colour));
      GL_CHECK(glVertexAttribPointer(colour, 4, GL_FLOAT, false, sizeof(AmjuGL::Vert), &tris[0].m_verts[0].m_r));
    }

    // Draw
    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, numTris * 3));

    // Cleanup
    if ((int)aPosition >= 0)
        GL_CHECK(glDisableVertexAttribArray(aPosition));

    if ((int)aNormal >= 0)
        GL_CHECK(glDisableVertexAttribArray(aNormal));

    if ((int)aUV >= 0)
        GL_CHECK(glDisableVertexAttribArray(aUV));

    if ((int)tan >= 0)
        GL_CHECK(glDisableVertexAttribArray(tan));

    if ((int)colour >= 0)
      GL_CHECK(glDisableVertexAttribArray(colour));

  }
  else
  {
    // Fixed function - TODO deprecate

    GL_CHECK(glEnableClientState(GL_VERTEX_ARRAY));
    GL_CHECK(glEnableClientState(GL_NORMAL_ARRAY));
    if (s_tt == AmjuGL::AMJU_TEXTURE_REGULAR)
    {
      // Don't specify tex coords if sphere map
      GL_CHECK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
      GL_CHECK(glTexCoordPointer(2, GL_FLOAT, sizeof(AmjuGL::Vert), &(tris[0].m_verts[0].m_u)));
    }

    GL_CHECK(glVertexPointer(3, GL_FLOAT, sizeof(AmjuGL::Vert), &(tris[0].m_verts[0].m_x)));
    GL_CHECK(glNormalPointer(GL_FLOAT, sizeof(AmjuGL::Vert), &(tris[0].m_verts[0].m_nx)));

#ifdef VERTEX_COLOURS
    // TODO This overwrites the current colour, so we must implement as a shader, where
    //  we can combine a uniform colour for the tri list, with per-vertex colours.
    GL_CHECK(glEnableClientState(GL_COLOR_ARRAY));
    GL_CHECK(glColorPointer(4, GL_FLOAT, sizeof(AmjuGL::Vert), &(tris[0].m_verts[0].m_r)));
#endif

    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, numTris * 3));

    GL_CHECK(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
    GL_CHECK(glDisableClientState(GL_COLOR_ARRAY));
    GL_CHECK(glDisableClientState(GL_NORMAL_ARRAY));
    GL_CHECK(glDisableClientState(GL_VERTEX_ARRAY));
  }
}

void AmjuGLOpenGL::DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2)
{
  AMJU_CALL_STACK;

  if (!s_shader)
  {
    // Disable texturing while drawing line ?
    //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
    glBegin(GL_LINES);
    // Apparently we shouldn't call glGetError() between a glBegin and glEnd?!
    glVertex3f(v1.m_x, v1.m_y, v1.m_z);
    glVertex3f(v2.m_x, v2.m_y, v2.m_z);
    glEnd();
    GL_CHECK(;);
    //AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  }
}

void AmjuGLOpenGL::DrawIndexedTriList(
  const AmjuGL::Verts& verts,
  const AmjuGL::IndexedTriList& indexes)
{
  AMJU_CALL_STACK;

  // TODO
}

void AmjuGLOpenGL::GetMatrix(AmjuGL::MatrixMode m, float result[16])
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

void AmjuGLOpenGL::SetMatrixMode(AmjuGL::MatrixMode m)
{
  AMJU_CALL_STACK;

  switch (m)
  {
  case AmjuGL::AMJU_MODELVIEW_MATRIX:
    GL_CHECK(glMatrixMode(GL_MODELVIEW));
    return;    

  case AmjuGL::AMJU_PROJECTION_MATRIX:
    GL_CHECK(glMatrixMode(GL_PROJECTION));
    return;

  case AmjuGL::AMJU_TEXTURE_MATRIX:
    GL_CHECK(glMatrixMode(GL_TEXTURE));
    return;

  default:
    // TODO Assert/print error
    break;
  }
}

void AmjuGLOpenGL::SetIdentity()
{
  AMJU_CALL_STACK;

  GL_CHECK(glLoadIdentity());
}

void AmjuGLOpenGL::PushMatrix()
{
  AMJU_CALL_STACK;

  GL_CHECK(glPushMatrix());
}

void AmjuGLOpenGL::PopMatrix()
{
  AMJU_CALL_STACK;

  GL_CHECK(glPopMatrix());
}

void AmjuGLOpenGL::Translate(float x, float y, float z)
{
  AMJU_CALL_STACK;

  GL_CHECK(glTranslatef(x, y, z));
}

void AmjuGLOpenGL::Scale(float x, float y, float z)
{
  AMJU_CALL_STACK;

  GL_CHECK(glScalef(x, y, z));
}

void AmjuGLOpenGL::RotateX(float degs)
{
  AMJU_CALL_STACK;

  GL_CHECK(glRotatef(degs, 1, 0, 0));
}

void AmjuGLOpenGL::RotateY(float degs)
{
  AMJU_CALL_STACK;

  GL_CHECK(glRotatef(degs, 0, 1, 0));
}

void AmjuGLOpenGL::RotateZ(float degs)
{
  AMJU_CALL_STACK;

  GL_CHECK(glRotatef(degs, 0, 0, 1));
}

void AmjuGLOpenGL::MultMatrix(const float matrix[16])
{
  AMJU_CALL_STACK;

  GL_CHECK(glMultMatrixf(matrix)); 
}

void AmjuGLOpenGL::SetTextureType(AmjuGL::TextureType tt)
{
  AMJU_CALL_STACK;

  s_tt = tt;

  if (tt == AmjuGL::AMJU_TEXTURE_REGULAR)
  {
    GL_CHECK(glDisable(GL_TEXTURE_GEN_S));
    GL_CHECK(glDisable(GL_TEXTURE_GEN_T));
  }
  else if (tt == AmjuGL::AMJU_TEXTURE_SPHERE_MAP)
  {
    GL_CHECK(glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP));
    GL_CHECK(glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP));
    GL_CHECK(glEnable(GL_TEXTURE_GEN_S));
    GL_CHECK(glEnable(GL_TEXTURE_GEN_T));
  }
}

void AmjuGLOpenGL::SetTexture(
  AmjuGL::TextureHandle* th, 
  AmjuGL::TextureType tt, 
  AmjuGL::TextureDepth d, 
  int width, 
  int height, 
  uint8* data)
{
  AMJU_CALL_STACK;

  GL_CHECK(glGenTextures(1, (GLuint*)th));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, *th));

  bool wrap = true; // TODO

  GL_CHECK(glEnable(GL_TEXTURE_2D));

  int wrapmode = GL_REPEAT;
  if (!wrap)
  {
    wrapmode = GL_CLAMP;
  }

  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmode));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmode));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

  GL_CHECK(glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE));

  // Different depending on AMJU_RGB or RGBA.
  if (d == AmjuGL::AMJU_RGB)
  {
      GL_CHECK(glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGB, // or 3 works
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        data));
  }
  else //if (d == RGBA)
  {
      GL_CHECK(glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA, // or 4 works // four components, not 3 - i.e., RGBA.
        width,
        height,
        0,
        GL_RGBA, // not just AMJU_RGB.
        GL_UNSIGNED_BYTE,
        data));
  }

  // Build mipmaps 
  if (d == AmjuGL::AMJU_RGB)
  {
    GL_CHECK(gluBuild2DMipmaps(
      GL_TEXTURE_2D,
      GL_RGB,
      width,
      height,
      GL_RGB,
      GL_UNSIGNED_BYTE,
      data));
  }
  else
  {
    GL_CHECK(gluBuild2DMipmaps(
      GL_TEXTURE_2D,
      GL_RGBA,
      width,
      height,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      data));
  }
}

void AmjuGLOpenGL::GetScreenshot(unsigned char* buffer, int x, int y, int w, int h)
{
  AMJU_CALL_STACK;

  GL_CHECK(glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer));
}

Shader* AmjuGLOpenGL::LoadShader(const std::string& shaderFileName)
{
  AMJU_CALL_STACK;

  GLShader* s = new GLShader;
  // TODO Add "ogl" to shader file name
  // TODO Two separate files for frag and vertex
  if (!s->Load(shaderFileName))
  {
    //ReportError("Failed to load shader " + shaderFileName);
    delete s;
    return 0; //new ShaderNull;
  }
  return s;

  return 0;
}

void AmjuGLOpenGL::UseShader(Shader* sh)
{
  GLShader* glshader = dynamic_cast<GLShader*>(sh);
  if (sh)
  {
    Assert(glshader);
    ////glUseProgram(glshader->GetProgHandle());
    sh->UseThisShader();
  }  
  else
  {
    GL_CHECK(glUseProgram(0));
  }
  s_shader = glshader;
}
}

#endif // defined (WIN32) || defined (MACOSX)

