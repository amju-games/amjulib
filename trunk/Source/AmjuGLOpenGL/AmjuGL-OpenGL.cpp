/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#include <AmjuFirst.h>
#if defined (WIN32) || defined (MACOSX)

#ifdef WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif

#include <iostream>
//#include "GL/glew.h"
#include <TriList.h>
#include "AmjuGL-OpenGL.h"
#include "GLShader.h"
#include "ShaderNull.h"
#include "AmjuAssert.h"
#include "OpenGL.h"
#include "ShadowMapOpenGL1.h"
#include "ShadowMapOpenGL2.h"
#include "ShadowMapOpenGL3.h"
#include <AmjuFinal.h>

#define SHADER_DEBUG
//#define USE_IMMEDIATE_MODE
//#define OPENGL_SHOW_INFO

#undef CreateWindow

namespace Amju
{
static DrawableFactory s_factory;

class TriListStaticOpenGL : public TriListStatic
{
public:
  // TODO Use display list
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

static Drawable* MakeShadowMap()
{
  // TODO: create best quality impl depending on hardware capability
  if (!glBindFramebufferEXT)
  {
    // Required extension not supported
    return new ShadowMapNull;
  }
  return new ShadowMapOpenGL2;
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

  glColor4f(r, g, b, a);
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

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glEnable(GL_LIGHT0);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gAmbient);
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

void AmjuGLOpenGL::Init()
{
  AMJU_CALL_STACK;

  AmjuGLOpenGLBase::Init();

  glEnable(GL_COLOR_MATERIAL); 

  // TODO Does glew work with GLES ?
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
}

void AmjuGLOpenGL::SetPerspectiveProjection(
  float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  gluPerspective(fov, aspectRatio, nearDist, farDist);
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
  }
  return 0;
}

void AmjuGLOpenGL::Enable(uint32 flag)
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

void AmjuGLOpenGL::Disable(uint32 flag)
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

void AmjuGLOpenGL::DrawTriList(const AmjuGL::Tris& tris)
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

void AmjuGLOpenGL::DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2)
{
  AMJU_CALL_STACK;

  // Disable texturing while drawing line ?
  //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  glBegin(GL_LINES);
  glVertex3f(v1.m_x, v1.m_y, v1.m_z);
  glVertex3f(v2.m_x, v2.m_y, v2.m_z);
  glEnd();
  //AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
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

void AmjuGLOpenGL::SetIdentity()
{
  AMJU_CALL_STACK;

  glLoadIdentity();
}

void AmjuGLOpenGL::PushMatrix()
{
  AMJU_CALL_STACK;

  glPushMatrix();
}

void AmjuGLOpenGL::PopMatrix()
{
  AMJU_CALL_STACK;

  glPopMatrix();
}

void AmjuGLOpenGL::Translate(float x, float y, float z)
{
  AMJU_CALL_STACK;

  glTranslatef(x, y, z);
}

void AmjuGLOpenGL::Scale(float x, float y, float z)
{
  AMJU_CALL_STACK;

  glScalef(x, y, z);
}

void AmjuGLOpenGL::RotateX(float degs)
{
  AMJU_CALL_STACK;

  glRotatef(degs, 1, 0, 0);
}

void AmjuGLOpenGL::RotateY(float degs)
{
  AMJU_CALL_STACK;

  glRotatef(degs, 0, 1, 0);
}

void AmjuGLOpenGL::RotateZ(float degs)
{
  AMJU_CALL_STACK;

  glRotatef(degs, 0, 0, 1);
}

void AmjuGLOpenGL::MultMatrix(const float matrix[16])
{
  AMJU_CALL_STACK;

  glMultMatrixf(matrix); 
}

void AmjuGLOpenGL::SetTextureType(AmjuGL::TextureType tt)
{
  AMJU_CALL_STACK;

  s_tt = tt;

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

  glGenTextures(1, (GLuint*)th);
  glBindTexture(GL_TEXTURE_2D, *th);

  bool wrap = true; // TODO

  glEnable(GL_TEXTURE_2D);

  int wrapmode = GL_REPEAT;
  if (!wrap)
  {
    wrapmode = GL_CLAMP;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmode);
  // New code: changed to use mipmaps
  //  GL_LINEAR_MIPMAP_LINEAR is supposedly the smoothest-looking.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  // Allowed values for min filter:
  // GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST,
  // GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
/*
  Don't need this
  if (tt == AmjuGL::AMJU_TEXTURE_REGULAR)
  {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  }
  else if (tt == AmjuGL::AMJU_TEXTURE_SPHERE_MAP)
  {
    // (Why) do we need to copy this when using the texture ?
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  }
*/
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // Different depending on AMJU_RGB or RGBA.
  if (d == AmjuGL::AMJU_RGB)
  {
      glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGB, // or 3 works
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
        GL_RGBA, // or 4 works // four components, not 3 - i.e., RGBA.
        width,
        height,
        0,
        GL_RGBA, // not just AMJU_RGB.
        GL_UNSIGNED_BYTE,
        data);
  }

  // Build mipmaps 
  if (d == AmjuGL::AMJU_RGB)
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
}

void AmjuGLOpenGL::GetScreenshot(unsigned char* buffer, int w, int h)
{
  AMJU_CALL_STACK;

  glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer);
}

Shader* AmjuGLOpenGL::LoadShader(const std::string& shaderFileName)
{
  AMJU_CALL_STACK;

  GLShader* s = new GLShader;
  // TODO Add "ogl" to shader file name
  // TODO Two separate files for frag and vertex
  if (!s->Load(shaderFileName))
  {
    delete s;
    return new ShaderNull;
  }
  return s;

  return 0;
}
}

#endif // defined (WIN32) || defined (MACOSX)

