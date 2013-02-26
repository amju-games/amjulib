/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#if defined (WIN32) || defined (MACOSX)

#ifdef WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif

#include "AmjuFirst.h"
#include <iostream>
//#include "GL/glew.h"
#include "AmjuGL-OpenGL.h"
#include "GLShader.h"
#include "ShaderNull.h"
#include "AmjuAssert.h"
#include "OpenGL.h"
#include "AmjuFinal.h"

#define SHADER_DEBUG
//#define USE_IMMEDIATE_MODE
//#define OPENGL_SHOW_INFO

#undef CreateWindow

namespace Amju
{
// Remember the current texture type. If sphere mapped, no need to send
// texture coords to the graphics card.
static AmjuGL::TextureType s_tt = AmjuGL::AMJU_TEXTURE_REGULAR;

AmjuGLOpenGL::WindowCreateFunc windowCreateFunc = 0;

AmjuGLOpenGL::AmjuGLOpenGL(AmjuGLOpenGL::WindowCreateFunc f)
{
  windowCreateFunc = f;
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

void AmjuGLOpenGL::Init()
{
  AMJU_CALL_STACK;

  AmjuGLOpenGLBase::Init();

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

/*
void AmjuGLOpenGL::DrawQuadList(const AmjuGL::Quads& quads)
{
  AMJU_CALL_STACK;

  int numQuads = quads.size();

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  if (s_tt == AmjuGL::AMJU_TEXTURE_REGULAR)
  {
    // Don't specify tex coords if sphere map
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(AmjuGL::Vert), &(quads[0].m_verts[0].m_u));
  }

  glVertexPointer(3, GL_FLOAT, sizeof(AmjuGL::Vert), &(quads[0].m_verts[0].m_x));
  glNormalPointer(GL_FLOAT, sizeof(AmjuGL::Vert), &(quads[0].m_verts[0].m_nx)); 
  glDrawArrays(GL_QUADS, 0, numQuads * 4);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void AmjuGLOpenGL::DrawQuad(AmjuGL::Vert* verts)
{
  AMJU_CALL_STACK;

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3, GL_FLOAT, sizeof(AmjuGL::Vert), &(verts[0].m_x));
  glNormalPointer(GL_FLOAT, sizeof(AmjuGL::Vert), &(verts[0].m_nx)); 
  glTexCoordPointer(2, GL_FLOAT, sizeof(AmjuGL::Vert), &(verts[0].m_u));

  glDrawArrays(GL_QUADS, 0, 4);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

#ifdef USE_IMMEDIATE_MODE
  glBegin(GL_QUADS);
  
  for (int i = 0; i < 4; i++)
  {
    glTexCoord2fv(&verts[i].m_u); 
    glNormal3fv(&verts[i].m_nx);
    glVertex3fv(&verts[i].m_x);
  }

  glEnd();
#endif // USE_IMMEDIATE_MODE
}
*/

void AmjuGLOpenGL::DrawIndexedTriList(
  const AmjuGL::Verts& verts,
  const AmjuGL::IndexedTriList& indexes)
{
  AMJU_CALL_STACK;

  // TODO
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
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

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

