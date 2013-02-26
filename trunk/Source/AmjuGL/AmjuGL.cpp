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
#include <iostream>
#include <StringUtils.h> // For debug reporting state
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

#ifdef AMJU_PUSH_POP_CHECK
static const char* AMJU_MTX_NAME[] = 
{
  "MODVIEW", "PROJ", "TEXTURE"
};
#endif

static float s_screenRotation = 0;

static Colour clearCol;

// Current colour, for reporting
static Colour s_colour;
// Viewport, for reporting
int viewport[4] = { -1, -1, -1, -1 };

// Consistency checks
static int initCalled = 0;
static int initFrameCalled = 0;
static int flipCalled = 0;

AmjuGL::Vert::Vert(float x, float y, float z, float u, float v, float nx, float ny, float nz)
  : m_x(x), m_y(y), m_z(z), m_nx(nx), m_ny(ny), m_nz(nz), m_u(u), m_v(v) {}

static std::ostream& ReportCol(const Colour& c, std::ostream& os)
{
  return os << "R: " << c.m_r << " G: " << c.m_g << " B: " << c.m_b << " A: " << c.m_a;
}

#ifdef ENABLE_DISABLE_DEBUG
static const char* FlagStr(uint32 f)
{
  switch (f)
  {
  case AmjuGL::AMJU_DEPTH_READ:
    return "ZRead";
  case AmjuGL::AMJU_LIGHTING:
    return "Lighting";
  case AmjuGL::AMJU_BLEND:
    return "Blend";
  case AmjuGL::AMJU_TEXTURE_2D:
    return "Tex2d";
  case AmjuGL::AMJU_DEPTH_WRITE:
    return "ZWrite";
  default:
    Assert(0);
  }
  return 0;
}
#endif

// TODO just use sprintf, to remove dep on StringUtils
static std::ostream& ReportMatrix(float m[16], std::ostream& os)
{
  for (int i = 0; i < 4; i++)
  {
    os << ToString(m[i], 2) << "\t" << ToString(m[i + 4], 2) << "\t" << ToString(m[i + 8], 2) << "\t" << ToString(m[i + 12], 2) << "\n";
  }
  return os;
}

std::ostream& AmjuGL::ReportState(std::ostream& os)
{
  os << "AmjuGL state\n"
     << "Lighting: " << (currentFlags & AMJU_LIGHTING ? "E" : "D") << "  "
     << "ZRead: " << (currentFlags & AMJU_DEPTH_READ ? "E" : "D") << "  "
     << "ZWrite: " << (currentFlags & AMJU_DEPTH_WRITE ? "E" : "D") << "  "
     << "Blend: " << (currentFlags & AMJU_BLEND ? "E" : "D") << "  "
     << "Tex2D: " << (currentFlags & AMJU_TEXTURE_2D ? "E" : "D") << "  "
     << "\n";
     
  os << "Colour: ";
  ReportCol(s_colour, os);
  os << "\n";

  float m[16];
  GetMatrix(AMJU_MODELVIEW_MATRIX, m);
  os << "Modelview matrix:\n";
  ReportMatrix(m, os);
  GetMatrix(AMJU_PROJECTION_MATRIX, m);
  os << "Projection matrix:\n";
  ReportMatrix(m, os);
 
  os << "Viewport: " << viewport[0] << " " << viewport[1] << " " << viewport[2] << " " << viewport[3] << "\n";

  return os;
}

// TODO Track usage, number of state changes per frame, etc


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

  initFrameCalled = 0; // Reset for next frame
  flipCalled++; 
  Assert(flipCalled == 1);

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
  initCalled++;
  Assert(initCalled == 1);

  currentFlags = AMJU_DEPTH_WRITE; // on at startup, right ?
 
  impl->Init();
	
#ifndef IPHONE	
  Viewport(0, 0, Screen::X(), Screen::Y());
#endif
}

bool AmjuGL::CreateWindow(AmjuGLWindowInfo* w)
{
  AMJU_CALL_STACK;
  
  Assert(initCalled == 0);

  Screen::SetSize(w->GetWidth(), w->GetHeight());
  bool b = impl->CreateWindow(w);
  return b;
}

void AmjuGL::SetClearColour(const Colour& col)
{
  clearCol = col;
}

void AmjuGL::InitFrame()
{
  AMJU_CALL_STACK;
  Assert(initCalled == 1);
  initFrameCalled++;
  Assert(initFrameCalled == 1);
  flipCalled = 0; 

  impl->InitFrame(clearCol.m_r, clearCol.m_g, clearCol.m_b);
}

void AmjuGL::SetPerspectiveProjection(
  float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;

  impl->SetPerspectiveProjection(fov, aspectRatio, nearDist, farDist);
}

void AmjuGL::GetViewport(int* x, int* y, int* w, int* h)
{
  *x = viewport[0];
  *y = viewport[1];
  *w = viewport[2];
  *h = viewport[3];
}

void AmjuGL::Viewport(int x, int y, int w, int h)
{
  AMJU_CALL_STACK;

  Assert(w > 0);
  Assert(h > 0);

  // TODO "LANDSCAPE" should be settable at run time..?
  // Currently it's defined for devices that have a portrait orientation by default, i.e. phones.
  // (So why is it called LANDSCAPE, sigh....)

#ifdef LANDSCAPE
  if (viewport[0] == y && viewport[1] == x && viewport[2] == h && viewport[3] == w)
  {
    return;
  }

  impl->Viewport(y, x, h, w);

  viewport[0] = y;
  viewport[1] = x;
  viewport[2] = h;
  viewport[3] = w;

#else
  if (viewport[0] == x && viewport[1] == y && viewport[2] == w && viewport[3] == h)
  {
    return;
  }

  impl->Viewport(x, y, w, h);

  viewport[0] = x;
  viewport[1] = y;
  viewport[2] = w;
  viewport[3] = h;
#endif
 
#ifdef VIEWPORT_DEBUG 
  std::cout << "AmjuGL::Viewport called: x: " << x << " y: " << y << " w: " << w << " h: " << h << ".. "
#ifdef LANDSCAPE
    << " LANDSCAPE "
#else
    << " not landscape "
#endif
    << " mode, so viewport is: " << viewport[0] << " " << viewport[1] << " " << viewport[2] << " " << viewport[3] << "\n";
#endif // VIEWPORT_DEBUG
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

  s_colour = Colour(r, g, b, a); // for reporting
  impl->SetColour(r, g, b, a);
}

void AmjuGL::SetColour(const Colour& c)
{
  SetColour(c.m_r, c.m_g, c.m_b, c.m_a);
}

#ifdef _DEBUG
static bool s_isWireFrame = false;
bool AmjuGL::IsWireFrameMode()
{
  return s_isWireFrame;
}

void AmjuGL::SetIsWireFrameMode(bool wf)
{
  s_isWireFrame = wf;
}
#endif

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

#ifdef _DEBUG
  if (IsWireFrameMode())
  {
    for (unsigned int i = 0; i < tris.size(); i++)
    {
      const Tri& t = tris[i];
      Vec3 v0(t.m_verts[0].m_x, t.m_verts[0].m_y, t.m_verts[0].m_z);
      Vec3 v1(t.m_verts[1].m_x, t.m_verts[1].m_y, t.m_verts[1].m_z);
      Vec3 v2(t.m_verts[2].m_x, t.m_verts[2].m_y, t.m_verts[2].m_z);

      DrawLine(v0, v1);
      DrawLine(v1, v2);
      DrawLine(v2, v0);
    }
  }
  else
#endif

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

  // Whole screen rotation
  if (GetMatrixMode() == AMJU_PROJECTION_MATRIX)
  {
    RotateZ(GetScreenRotation());
  }
}

void AmjuGL::PushMatrix()
{
  AMJU_CALL_STACK;

  Assert(s_currentMatrix != AMJU_MATRIX_NOTSETYET);

#ifdef _DEBUG
  s_matrixStackSize[s_currentMatrix]++;

#ifdef AMJU_PUSH_POP_CHECK
  if (s_currentMatrix != AMJU_MODELVIEW_MATRIX)
  {
    std::cout << "PushMatrix called for " 
      << (s_currentMatrix == AMJU_PROJECTION_MATRIX ? "PROJ" : "TEXTURE") << " matrix\n";
  }

  std::cout << "Push: Stack size for " << AMJU_MTX_NAME[s_currentMatrix] << ": " << 
    s_matrixStackSize[s_currentMatrix] << "\n";

#endif // AMJU_PUSH_POP_CHECK
#endif // _DEBUG

  impl->PushMatrix();
}

void AmjuGL::PopMatrix()
{
  AMJU_CALL_STACK;

  Assert(s_currentMatrix != AMJU_MATRIX_NOTSETYET);

#ifdef _DEBUG
  Assert(s_matrixStackSize[s_currentMatrix] > 0);
  s_matrixStackSize[s_currentMatrix]--;

#ifdef AMJU_PUSH_POP_CHECK
  if (s_currentMatrix != AMJU_MODELVIEW_MATRIX)
  {
    std::cout << "PopMatrix called for " 
      << (s_currentMatrix == AMJU_PROJECTION_MATRIX ? "PROJ" : "TEXTURE") << " matrix\n";
  }

  std::cout << "Pop: Stack size for " << AMJU_MTX_NAME[s_currentMatrix] << ": " << 
    s_matrixStackSize[s_currentMatrix] << "\n";

#endif // AMJU_PUSH_POP_CHECK
#endif // _DEBUG

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
#ifdef ENABLE_DISABLE_DEBUG
std::cout << "Enable: Flag " << FlagStr(flags) << " already enabled, so ignored\n"; 
#endif
    return;
  }
  currentFlags |= flags;
  impl->Enable(flags);
#ifdef ENABLE_DISABLE_DEBUG
std::cout << "Enable: Flag " << FlagStr(flags) << " enabled\n"; 
#endif
}

void AmjuGL::Disable(uint32 flags)
{
  AMJU_CALL_STACK;

  if (!(currentFlags & flags))
  {
#ifdef ENABLE_DISABLE_DEBUG
std::cout << "Disable: Flag " << FlagStr(flags) << " already disabled, so ignored\n"; 
#endif
    return;
  }
  currentFlags &= ~flags;
  impl->Disable(flags);
#ifdef ENABLE_DISABLE_DEBUG
std::cout << "Disable: Flag " << FlagStr(flags) << " disabled\n"; 
#endif
}

void AmjuGL::DestroyTextureHandle(TextureHandle* th)
{
  AMJU_CALL_STACK;

  impl->DestroyTextureHandle(th);
}

void AmjuGL::SetTextureType(TextureType tt)
{
  AMJU_CALL_STACK;

  impl->SetTextureType(tt);
}

void AmjuGL::SetTextureMode(TextureMode tm)
{
  impl->SetTextureMode(tm);
}

void AmjuGL::SetTextureFilter(TextureFilter tf)
{
  impl->SetTextureFilter(tf);
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
  prev = t;

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

ShadowMap* AmjuGL::CreateShadowMap()
{
  AMJU_CALL_STACK;
  return impl->CreateShadowMap();
}

Drawable* AmjuGL::Create(int drawableTypeId)
{
  AMJU_CALL_STACK;
  return impl->Create(drawableTypeId);
}

}

