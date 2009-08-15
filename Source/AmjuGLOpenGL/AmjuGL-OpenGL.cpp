/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#ifdef WIN32
#pragma warning(disable: 4786)
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif
#include "AmjuFirst.h"
#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>
#include "GL/glew.h"
#include "AmjuGL-OpenGL.h"
#include "AmjuAssert.h"
#include "OpenGL.h"
#include "AmjuFinal.h"

#define SHADER_DEBUG

namespace Amju
{
// Remember the current texture type. If sphere mapped, no need to send
// texture coords to the graphics card.
static AmjuGL::TextureType s_tt = AmjuGL::AMJU_TEXTURE_REGULAR;


OglShader::OglShader()
{
  AMJU_CALL_STACK;

  m_vertexShaderHandle = 0;
  m_fragmentShaderHandle = 0;
  m_programHandle = 0;
} 

OglShader::~OglShader()
{
  AMJU_CALL_STACK;

}

bool OglShader::Load(const std::string& vertexSource, const std::string& fragmentSource)
{
  AMJU_CALL_STACK;

  if (!AmjuGLOpenGL::s_shaderSupport)
  {
    m_errorStr = "GLSL not supported";
    return false;
  }

#ifdef SHADER_DEBUG
std::cout << "Loading shader..\nVertex Shader source:\n" << vertexSource.c_str()
  << "\nFragment Shader source:\n" << fragmentSource.c_str() << "\n";
#endif

  m_vertexShaderHandle = glCreateShaderObjectARB(GL_VERTEX_SHADER);
  m_fragmentShaderHandle = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

#ifdef SHADER_DEBUG
std::cout << "Created shader handles\n";
#endif

  const GLint vlength = vertexSource.size();
  const GLint flength = fragmentSource.size();
  const char* vStr = vertexSource.c_str();
  const char* fStr = fragmentSource.c_str();
  glShaderSourceARB(m_vertexShaderHandle, 1, &vStr, &vlength);
  glShaderSourceARB(m_fragmentShaderHandle, 1, &fStr, &flength);

#ifdef SHADER_DEBUG
std::cout << "Set shader source\n";
#endif

  GLint compiled = 0;
  GLcharARB buf[2000]; // error string buffer

  glCompileShaderARB(m_vertexShaderHandle);

  glGetObjectParameterivARB(m_vertexShaderHandle, GL_COMPILE_STATUS, &compiled);
  if (!compiled)
  {
    glGetInfoLogARB(m_vertexShaderHandle, 2000, 0, buf);
    m_errorStr = buf;

    return false;
  }        
#ifdef SHADER_DEBUG
std::cout << "Compiled vertex shader\n";
#endif

  glCompileShaderARB(m_fragmentShaderHandle);
  
  glGetObjectParameterivARB(m_fragmentShaderHandle, GL_COMPILE_STATUS, &compiled);
  if (!compiled)
  {
    glGetInfoLogARB(m_fragmentShaderHandle, 2000, 0, buf);
    m_errorStr = buf;

    return false;
  }        

#ifdef SHADER_DEBUG
std::cout << "Compiled fragment shader\n";
#endif

  m_programHandle = glCreateProgramObjectARB();

#ifdef SHADER_DEBUG
std::cout << "Created program\n";
#endif

  glAttachObjectARB(m_programHandle, m_vertexShaderHandle);
  glAttachObjectARB(m_programHandle, m_fragmentShaderHandle);

#ifdef SHADER_DEBUG
std::cout << "Attached shaders to program\n";
#endif

  glLinkProgramARB(m_programHandle);

#ifdef SHADER_DEBUG
std::cout << "Link called\n";
#endif

  GLint linked;
  glGetObjectParameterivARB(m_programHandle, GL_OBJECT_LINK_STATUS_ARB, &linked);
  if (!linked)
  {
    glGetInfoLogARB(m_programHandle, 2000, 0, buf);
    m_errorStr = buf;
    
    return false; 
  }        

#ifdef SHADER_DEBUG
std::cout << "Shader program linked OK!\n";
#endif

  return true;
}

std::string OglShader::GetErrorString()
{
  return m_errorStr; 
}

void OglShader::Begin()
{
  AMJU_CALL_STACK;

  glUseProgramObjectARB(m_programHandle);
}

void OglShader::End()
{
  AMJU_CALL_STACK;

  glUseProgramObjectARB(0);
}


bool AmjuGLOpenGL::s_shaderSupport = false;

void AmjuGLOpenGL::BeginScene()
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGL::EndScene()
{
  AMJU_CALL_STACK;
}

void AmjuGLOpenGL::Flip()
{
  AMJU_CALL_STACK;

#ifdef WIN32
  SwapBuffers(wglGetCurrentDC());
#endif
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

  // OpenGL version
  const unsigned char* version = glGetString(GL_VERSION);
  const unsigned char* vendor = glGetString(GL_VENDOR);
  const unsigned char* renderer = glGetString(GL_RENDERER);
  const unsigned char* extensions = glGetString(GL_EXTENSIONS);

#ifdef _DEBUG
std::cout << "OpenGL Version: " << version << "\n";
std::cout << "OpenGL Vendor: " << vendor << "\n";
std::cout << "OpenGL Renderer: " << renderer << "\n";
std::cout << "OpenGL Extensions: " << extensions << "\n";
#endif

  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    // failed to initialize GLEW!
    std::cout << "Failed to initialise GLEW!\n";
  }
#ifdef _DEBUG
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
#ifdef _DEBUG
std::cout << "GLSL v.1.0.51 is supported.\n";
#endif
    }
    else
    {
      // parse string sVersion to get major, minor, revision
#ifdef _DEBUG
std::cout << "GLSL " << sVersion << " is supported.\n";
#endif
    }
    s_shaderSupport = true;
  }
  else
  {
#ifdef _DEBUG
std::cout << "GLSL is NOT SUPPORTED :-(\n";
#endif
  }

  // Set the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // Try to disable blending unless expicitly required
  glDisable(GL_BLEND);

  // We can rely on default values for everything else.
  // NB Lighting is set up for each room when player enters.
}

void AmjuGLOpenGL::InitFrame(float clearR, float clearG, float clearB)
{
  AMJU_CALL_STACK;

  // Do GL initialisation before we draw the frame.
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_TEST);
  glClearColor(clearR, clearG, clearB, 1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  AmjuGL::Disable(GL_BLEND); // only enabled where necessary
  // NB Must use AmjuGL::Disable so flags are set consistently!

  //glEnable(GL_BLEND); // TODO some things (day night sky etc) must enable this.
  glEnable(GL_TEXTURE_2D);

  // Set the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void AmjuGLOpenGL::SetPerspectiveProjection(
  float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, aspectRatio, nearDist, farDist);

// ???
//  glMatrixMode(GL_MODELVIEW);
}

void AmjuGLOpenGL::SetOrthoProjection()
{
  AMJU_CALL_STACK;

  // TODO
}

void AmjuGLOpenGL::Viewport(int x, int y, int w, int h)
{
  AMJU_CALL_STACK;

  if (w != 0 && h != 0)
  {
    glViewport(x, y, w, h);
  }
}

void AmjuGLOpenGL::LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ)
{
  AMJU_CALL_STACK;

  gluLookAt(eyeX, eyeY, eyeZ, // origin - player coords
            x, y, z, // point in direction we want to look
            upX, upY, upZ /* 'Up' vector */);
}

void AmjuGLOpenGL::SetColour(float r, float g, float b, float a)
{
  AMJU_CALL_STACK;

  glColor4f(r, g, b, a);
}

void AmjuGLOpenGL::DrawTriList(const AmjuGL::Tris& tris)
{
  AMJU_CALL_STACK;

  int numTris = tris.size();

  // Argh, format is different for DX9 verts and OpenGL verts :-(
  // So it looks like we can't do
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


#ifdef USE_IMMEDIATE_MODE
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

#ifdef _DEBUG
  // Disable texturing while drawing line
  glDisable(GL_TEXTURE_2D);
  glBegin(GL_LINES);
  glVertex3f(v1.m_x, v1.m_y, v1.m_z);
  glVertex3f(v2.m_x, v2.m_y, v2.m_z);
  glEnd();
  glEnable(GL_TEXTURE_2D);
#endif // _DEBUG
}

void AmjuGLOpenGL::DrawSphere(const AmjuGL::Vec3& v, float r)
{
  AMJU_CALL_STACK;

#if defined(USE_GLUT)
    // Use glut wire sphere function.
    //glPushMatrix();
    //glLoadIdentity();
    //glTranslatef(v.m_x, v.m_y, v.m_z);
    glutWireSphere(r, 8, 8); // radius, slices, stacks
    //glPopMatrix();
#else
    // TODO non-GLUT sphere
#endif
}

void AmjuGLOpenGL::DrawQuadList(const AmjuGL::Quads& quads)
{
  AMJU_CALL_STACK;

  int numQuads = quads.size();

  // Argh, format is different for DX9 verts and OpenGL verts :-(
  // So it looks like we can't do
  //glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(AmjuGL::Vert), &quads[0]);
  //glDrawArrays(GL_QUADS, 0, numQuads * 4);

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

void AmjuGLOpenGL::DrawIndexedTriList(
  const AmjuGL::Verts& verts,
  const AmjuGL::IndexedTriList& indexes)
{
  AMJU_CALL_STACK;

  // TODO
}

void AmjuGLOpenGL::MultMatrix(const float matrix[16])
{
  AMJU_CALL_STACK;

  glMultMatrixf(matrix); 
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

void AmjuGLOpenGL::PushAttrib(uint32 attrib)
{
  AMJU_CALL_STACK;

  // Push/Pop Attrib is emulated in AmjuGL -- Enable/Disable is called, 
  //  no need to use glPushAttrib/glPopAttrib.
  // This is for consistency and also is good for OpenGL ES.
}

void AmjuGLOpenGL::PopAttrib()
{
  AMJU_CALL_STACK;

  //glPopAttrib();
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
  case AmjuGL::AMJU_DEPTH_TEST:
    return GL_DEPTH_TEST;
  }
  return 0;
}

void AmjuGLOpenGL::Enable(uint32 flag)
{
  AMJU_CALL_STACK;

  uint32 glFlag = ConvertToGLFlag(flag);
  glEnable(glFlag);
}

void AmjuGLOpenGL::Disable(uint32 flag)
{
  AMJU_CALL_STACK;

  uint32 glFlag = ConvertToGLFlag(flag);
  glDisable(glFlag);
}

void AmjuGLOpenGL::BlendFunc()
{
  AMJU_CALL_STACK;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void AmjuGLOpenGL::EnableZWrite(bool b)
{
  AMJU_CALL_STACK;

  glDepthMask(b ? GL_TRUE : GL_FALSE);
}

void AmjuGLOpenGL::DestroyTextureHandle(AmjuGL::TextureHandle* th)
{
  AMJU_CALL_STACK;

  int texId = (int)(*th);
  if (texId < 0)
  {
    std::cout << "\n*** Texture handle not found: " << texId << "\n";
    Assert(0);
  }

  // This crashes in release mode ?!
//  glDeleteTextures(1, (GLuint*)th);
}

void AmjuGLOpenGL::SetTextureMode(AmjuGL::TextureType tt)
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

#ifndef AMJU_GL_NONE
  glGenTextures(1, (GLuint*)th);
#endif

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

void AmjuGLOpenGL::UseTexture(AmjuGL::TextureHandle t)
{
  AMJU_CALL_STACK;

  glBindTexture(GL_TEXTURE_2D, t);
}

AmjuGL::Vec3 AmjuGLOpenGL::MouseToWorld(int x, int y)
{
  GLint viewport[4];
  GLdouble modelview[16],projection[16];
  GLfloat wx=(float)x,wy = 0;
  GLfloat wz = 0;

  // NB Order: get the viewport, then invert y, then get depth value :-)
  glGetIntegerv(GL_VIEWPORT,viewport);
  y=viewport[3]-y;

  glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);

  wy = (float)y;

  glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
  glGetDoublev(GL_PROJECTION_MATRIX,projection);
  double ox, oy, oz;
  gluUnProject(wx, wy, wz,
    modelview,projection,viewport,&ox,&oy,&oz);

  return AmjuGL::Vec3((float)ox, (float)oy, (float)oz);
}

void AmjuGLOpenGL::GetScreenshot(unsigned char* buffer, int w, int h)
{
  AMJU_CALL_STACK;

  glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer);
}
}


