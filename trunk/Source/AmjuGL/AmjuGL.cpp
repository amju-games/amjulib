/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#include "AmjuFirst.h"
#include <stack>
#include "AmjuGL.h"
#include "AmjuGL-Impl.h"
#include "AmjuGLWindowInfo.h"
#include "Screen.h"
#include "Colour.h"
#include "AmjuFinal.h"

namespace Amju
{
// Implement OpenGL-style Attribute stack in this base class.
typedef std::stack<uint32> AttribStack;
static AttribStack attribStack;
static uint32 currentFlags = 0;

// Not RCPtr - we don't want to destroy the impl before cleaning up all textures etc!
AmjuGLImpl* impl = 0;

#ifdef _DEBUG
// Store the size of the matrix stack for each matrix mode.
// Then we can check that the stacks are empty when we start and end drawing.
static int s_matrixStackSize[3] = { 0, 0, 0 };
#endif // _DEBUG

// Store the current matrix mode so we know which count to increment/decrement,
//  and for queries
static AmjuGL::MatrixMode s_currentMatrix = AmjuGL::AMJU_MATRIX_NOTSETYET;

static float s_screenRotation = 0;

AmjuGL::Vert::Vert(float x, float y, float z, float u, float v, float nx, float ny, float nz)
  : m_x(x), m_y(y), m_z(z), m_nx(nx), m_ny(ny), m_nz(nz), m_u(u), m_v(v) {}

void AmjuGL::SetImpl(AmjuGLImpl* i)
{
  AMJU_CALL_STACK;

  impl = i;
}

void AmjuGL::BeginScene()
{
  AMJU_CALL_STACK;

#ifdef _DEBUG
  for (int i = 0; i < 3; i++)
  {
    Assert(s_matrixStackSize[i] == 0);
  }
#endif

  impl->BeginScene();
}

void AmjuGL::EndScene()
{
  AMJU_CALL_STACK;

#ifdef _DEBUG
  for (int i = 0; i < 3; i++)
  {
    Assert(s_matrixStackSize[i] == 0);
  }
#endif

  impl->EndScene();
}

void AmjuGL::Flip()
{
  AMJU_CALL_STACK;

  impl->Flip();
}

void AmjuGL::DrawLighting(
  const AmjuGL::LightColour& globalAmbient, 
  const AmjuGL::LightColour& lightAmbient,
  const AmjuGL::LightColour& lightDiffuse,
  const AmjuGL::LightColour& lightSpecular,
  const AmjuGL::Vec3& lightPos)
{
  AMJU_CALL_STACK;

  impl->DrawLighting(globalAmbient, lightAmbient, lightDiffuse, lightSpecular, lightPos);
}

void AmjuGL::Init()
{
  AMJU_CALL_STACK;

  currentFlags = AMJU_DEPTH_WRITE; // on at startup, right ?
 
  impl->Init();
}

bool AmjuGL::CreateWindow(AmjuGLWindowInfo* w)
{
  AMJU_CALL_STACK;

  Screen::SetSize(w->GetWidth(), w->GetHeight());
  return impl->CreateWindow(w);
}

static Colour clearCol;
void AmjuGL::SetClearColour(const Colour& col)
{
  clearCol = col;
}

void AmjuGL::InitFrame()
{
  AMJU_CALL_STACK;

  impl->InitFrame(clearCol.m_r, clearCol.m_g, clearCol.m_b);
}

void AmjuGL::SetPerspectiveProjection(
  float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;

  impl->SetPerspectiveProjection(fov, aspectRatio, nearDist, farDist);
}

void AmjuGL::Viewport(int x, int y, int w, int h)
{
  AMJU_CALL_STACK;

#ifdef LANDSCAPE
  impl->Viewport(y, x, h, w);
#else
  impl->Viewport(x, y, w, h);
#endif
}

void AmjuGL::LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ)
{
  AMJU_CALL_STACK;

  impl->LookAt(eyeX, eyeY, eyeZ, // origin - player coords
            x, y, z, // point in direction we want to look
            upX, upY, upZ /* 'Up' vector */);
}

void AmjuGL::SetColour(float r, float g, float b, float a)
{
  AMJU_CALL_STACK;

  impl->SetColour(r, g, b, a);
}

void AmjuGL::SetColour(const Colour& c)
{
  SetColour(c.m_r, c.m_g, c.m_b, c.m_a);
}

void AmjuGL::DrawIndexedTriList(
  const Verts& verts,
  const IndexedTriList& indexes)
{
  Assert(0); // use tris

  AMJU_CALL_STACK;

  // TODO Dummy implementation
  // Create tris from indexed list
  // TODO  - AmjuGL implementation

  Tris tris;
  int numTris = indexes.m_indexes.size();
  tris.resize(numTris);
  for (int i = 0; i < numTris; i++)
  {
    Tri& t = tris[i];
    IndexedTriList::Tri it = indexes.m_indexes[i];
    for (int k = 0; k < 3; k++)
    {
      t.m_verts[k] = verts[it.m_index[k]];
    }
  }
  
  DrawTriList(tris);

//  impl->DrawIndexedTriList(verts, indexes);
}

void AmjuGL::DrawTriList(const Tris& tris)
{
  AMJU_CALL_STACK;

  impl->DrawTriList(tris);
}

void AmjuGL::DrawLine(const Vec3& v1, const Vec3& v2)
{
  AMJU_CALL_STACK;

  impl->DrawLine(v1, v2);
}

void AmjuGL::GetMatrix(MatrixMode m, float result[16])
{
  AMJU_CALL_STACK;

  impl->GetMatrix(m, result);
}

void AmjuGL::MultMatrix(const float matrix[16])
{
  AMJU_CALL_STACK;

  impl->MultMatrix(matrix);
}

AmjuGL::MatrixMode AmjuGL::GetMatrixMode()
{
  return s_currentMatrix;
}

void AmjuGL::SetMatrixMode(MatrixMode m)
{
  AMJU_CALL_STACK;

  if (s_currentMatrix == m)
  {
    return;
  }

  s_currentMatrix = m;

  impl->SetMatrixMode(m);
}

void AmjuGL::SetScreenRotation(float degs)
{
  AMJU_CALL_STACK;
  s_screenRotation = degs;
}

float AmjuGL::GetScreenRotation()
{
  AMJU_CALL_STACK;
  return s_screenRotation;
}

void AmjuGL::SetIdentity()
{
  AMJU_CALL_STACK;

  impl->SetIdentity();
}

void AmjuGL::PushMatrix()
{
  AMJU_CALL_STACK;

#ifdef _DEBUG
  s_matrixStackSize[s_currentMatrix]++;
#endif

  impl->PushMatrix();
}

void AmjuGL::PopMatrix()
{
  AMJU_CALL_STACK;

#ifdef _DEBUG
  Assert(s_matrixStackSize[s_currentMatrix] > 0);
  s_matrixStackSize[s_currentMatrix]--;
#endif

  impl->PopMatrix();
}

void AmjuGL::Translate(float x, float y, float z)
{
  AMJU_CALL_STACK;

  impl->Translate(x, y, z);
}

void AmjuGL::Scale(float x, float y, float z)
{
  AMJU_CALL_STACK;

  impl->Scale(x, y, z);
}

void AmjuGL::RotateX(float degs)
{
  AMJU_CALL_STACK;

  impl->RotateX(degs);
}

void AmjuGL::RotateY(float degs)
{
  AMJU_CALL_STACK;

  impl->RotateY(degs);
}

void AmjuGL::RotateZ(float degs)
{
  AMJU_CALL_STACK;

  impl->RotateZ(degs);
}

void AmjuGL::PushAttrib(uint32 attrib)
{
  AMJU_CALL_STACK;

  attribStack.push(currentFlags);

  impl->PushAttrib(attrib);
}

void AmjuGL::PopAttrib()
{
  AMJU_CALL_STACK;

  Assert(!attribStack.empty());
  
  // Pop new flags, set each bit
  uint32 newFlags = attribStack.top();
  //int newStackSize = attribStack.size();
  attribStack.pop();

  static uint32 flagsToCheck[] = 
  {
    AmjuGL::AMJU_DEPTH_READ,
    AmjuGL::AMJU_LIGHTING,
    AmjuGL::AMJU_BLEND,
    AmjuGL::AMJU_TEXTURE_2D,
    AmjuGL::AMJU_DEPTH_WRITE
  };
  
  for (unsigned int i = 0; i < sizeof(flagsToCheck) / sizeof(uint32); i++)
  {
    uint32 f = flagsToCheck[i];
    if (newFlags & f) 
    {
      Enable(f);
    }
    else 
    {
      Disable(f);
    }
  }

  impl->PopAttrib();
}

void AmjuGL::Enable(uint32 flags)
{
  AMJU_CALL_STACK;

  if (currentFlags & flags)
  {
    return;
  }
  currentFlags |= flags;
  impl->Enable(flags);
}

void AmjuGL::Disable(uint32 flags)
{
  AMJU_CALL_STACK;

  if (!(currentFlags & flags))
  {
    return;
  }
  currentFlags &= ~flags;
  impl->Disable(flags);
}

void AmjuGL::DestroyTextureHandle(TextureHandle* th)
{
  AMJU_CALL_STACK;

  impl->DestroyTextureHandle(th);
}

void AmjuGL::SetTextureMode(TextureType tt)
{
  AMJU_CALL_STACK;

  impl->SetTextureMode(tt);
}

void AmjuGL::SetTexture(
  TextureHandle* th, 
  TextureType tt, 
  TextureDepth d, 
  int width, 
  int height, 
  uint8* data)
{
  AMJU_CALL_STACK;

  impl->SetTexture(th, tt, d, width, height, data);
}

void AmjuGL::UseTexture(TextureHandle t)
{
  AMJU_CALL_STACK;

  static TextureHandle prev = -1;
  if (prev == t)
  {
    return;
  }

  impl->UseTexture(t);
}

void AmjuGL::GetScreenshot(unsigned char* buffer, int w, int h)
{
  AMJU_CALL_STACK;

  impl->GetScreenshot(buffer, w, h);
}

Shader* AmjuGL::LoadShader(const std::string& shaderFileName)
{
  AMJU_CALL_STACK;
  return impl->LoadShader(shaderFileName);
}
}
