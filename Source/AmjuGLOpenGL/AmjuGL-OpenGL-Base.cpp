/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#ifdef WIN32
#pragma warning(disable: 4786)
#endif

#include "AmjuFirst.h"
#include <iostream>
#include "OpenGL.h"
#include "AmjuGL-OpenGL-Base.h"
#include "../Utils/AmjuAssert.h"
#include "AmjuFinal.h"

//#define SHADER_DEBUG

namespace Amju
{
// Remember the current texture type. If sphere mapped, no need to send
// texture coords to the graphics card.
static AmjuGL::TextureType s_tt = AmjuGL::AMJU_TEXTURE_REGULAR;

// Store the size of the matrix stack for each matrix mode.
// Then we can check that the stacks are empty when we start and end drawing.
static int s_matrixStackSize[3] = { 0, 0, 0 };

// Store the current matrix mode so we know which count to increment/decrement
static int s_currentMatrix = 0;


bool AmjuGLOpenGLBase::s_shaderSupport = false;

void AmjuGLOpenGLBase::BeginScene()
{
  AMJU_CALL_STACK;

#ifdef _DEBUG
  for (int i = 0; i < 3; i++)
  {
    Assert(s_matrixStackSize[i] == 0);
  }
#endif
}

void AmjuGLOpenGLBase::EndScene()
{
  AMJU_CALL_STACK;

#ifdef _DEBUG
  for (int i = 0; i < 3; i++)
  {
    Assert(s_matrixStackSize[i] == 0);
  }
#endif

#ifdef WIN32
  SwapBuffers(wglGetCurrentDC());
#endif
}

void AmjuGLOpenGLBase::DrawLighting(
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

void AmjuGLOpenGLBase::Init()
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

  // Set the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

//  glEnable(GL_CULL_FACE);
//  glCullFace(GL_BACK);

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // Try to disable blending unless expicitly required
  glDisable(GL_BLEND);

  // We can rely on default values for everything else.
  // NB Lighting is set up for each room when player enters.
}

void AmjuGLOpenGLBase::InitFrame(float clearR, float clearG, float clearB)
{
  AMJU_CALL_STACK;

  // Do GL initialisation before we draw the frame.
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_TEST);
  glClearColor(clearR, clearG, clearB, 1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//  glCullFace(GL_BACK);
//  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_BLEND); // only enabled where necessary
  //glEnable(GL_BLEND); // TODO some things (day night sky etc) must enable this.
  glEnable(GL_TEXTURE_2D);

  // Set the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void AmjuGLOpenGLBase::Viewport(int x, int y, int w, int h)
{
  AMJU_CALL_STACK;

  glViewport(x, y, w, h);
}

void AmjuGLOpenGLBase::SetColour(float r, float g, float b, float a)
{
  AMJU_CALL_STACK;

  glColor4f(r, g, b, a);
}

void AmjuGLOpenGLBase::DrawTriList(const AmjuGL::Tris& tris)
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

void AmjuGLOpenGLBase::DrawSphere(const AmjuGL::Vec3& v, float r)
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

void AmjuGLOpenGLBase::DrawIndexedTriList(
  const AmjuGL::Verts& verts,
  const AmjuGL::IndexedTriList& indexes)
{
  AMJU_CALL_STACK;

  // TODO
}

void AmjuGLOpenGLBase::GetMatrix(AmjuGL::MatrixMode m, float result[16])
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

void AmjuGLOpenGLBase::SetMatrixMode(AmjuGL::MatrixMode m)
{
  AMJU_CALL_STACK;

  switch (m)
  {
  case AmjuGL::AMJU_MODELVIEW_MATRIX:
    glMatrixMode(GL_MODELVIEW);
    s_currentMatrix = 0;
    return;    

  case AmjuGL::AMJU_PROJECTION_MATRIX:
    glMatrixMode(GL_PROJECTION);
    s_currentMatrix = 1;
    return;

  case AmjuGL::AMJU_TEXTURE_MATRIX:
    glMatrixMode(GL_TEXTURE);
    s_currentMatrix = 2;
    return;

  default:
    // TODO Assert/print error
    break;
  }
}

void AmjuGLOpenGLBase::SetIdentity()
{
  AMJU_CALL_STACK;

  glLoadIdentity();
}

void AmjuGLOpenGLBase::PushMatrix()
{
  AMJU_CALL_STACK;

  s_matrixStackSize[s_currentMatrix]++;
  glPushMatrix();
}

void AmjuGLOpenGLBase::PopMatrix()
{
  AMJU_CALL_STACK;

  s_matrixStackSize[s_currentMatrix]--;
  glPopMatrix();
}

void AmjuGLOpenGLBase::Translate(float x, float y, float z)
{
  AMJU_CALL_STACK;

  glTranslatef(x, y, z);
}

void AmjuGLOpenGLBase::Scale(float x, float y, float z)
{
  AMJU_CALL_STACK;

  glScalef(x, y, z);
}

void AmjuGLOpenGLBase::RotateX(float degs)
{
  AMJU_CALL_STACK;

  glRotatef(degs, 1, 0, 0);
}

void AmjuGLOpenGLBase::RotateY(float degs)
{
  AMJU_CALL_STACK;

  glRotatef(degs, 0, 1, 0);
}

void AmjuGLOpenGLBase::RotateZ(float degs)
{
  AMJU_CALL_STACK;

  glRotatef(degs, 0, 0, 1);
}

void AmjuGLOpenGLBase::PushAttrib(uint32 attrib)
{
  AMJU_CALL_STACK;

  // Push/Pop Attrib is emulated in AmjuGL -- Enable/Disable is called, 
  //  no need to use glPushAttrib/glPopAttrib.
  // This is for consistency and also is good for OpenGL ES.
}

void AmjuGLOpenGLBase::PopAttrib()
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

void AmjuGLOpenGLBase::Enable(uint32 flag)
{
  AMJU_CALL_STACK;

  uint32 glFlag = ConvertToGLFlag(flag);
  //ASSERT(glFlag);
  glEnable(glFlag);
}

void AmjuGLOpenGLBase::Disable(uint32 flag)
{
  AMJU_CALL_STACK;

  uint32 glFlag = ConvertToGLFlag(flag);
  //ASSERT(glFlag);
  glDisable(glFlag);
}

void AmjuGLOpenGLBase::BlendFunc()
{
  AMJU_CALL_STACK;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void AmjuGLOpenGLBase::EnableZWrite(bool b)
{
  AMJU_CALL_STACK;

  glDepthMask(b ? GL_TRUE : GL_FALSE);
}

void AmjuGLOpenGLBase::DestroyTextureHandle(AmjuGL::TextureHandle* th)
{
  AMJU_CALL_STACK;

#ifndef AMJU_GL_NONE
  // This crashes in release mode ?!
//  glDeleteTextures(1, (GLuint*)th);
#endif
}

void AmjuGLOpenGLBase::UseTexture(AmjuGL::TextureHandle t)
{
  AMJU_CALL_STACK;

#ifndef AMJU_GL_NONE
  glBindTexture(GL_TEXTURE_2D, t);
#endif
}
}


