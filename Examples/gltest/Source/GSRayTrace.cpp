#include "GSRayTrace.h"
#include <AmjuGL.h>

// Size of texture encoding scene

// Each row encodes one mesh
const int NUM_ROWS = 8;

// Each texel in a row encodes triangle data or material
const int NUM_PIXELS_PER_ROW = 8;

namespace Amju
{
GSRayTrace::GSRayTrace()
{
  m_name = "GPU Ray tracing";
  m_description = "Ray tracing in fragment shader.";
  //m_nextState=...
}

void GSRayTrace::Update()
{
  GSBase::Update();

}

void GSRayTrace::DrawScene()
{
  m_tex.UseThisTexture();
  m_fspp.DrawFullScreenQuad();
}

void GSRayTrace::CreateTweakMenu()
{
  GSBase::CreateTweakMenu();
}

void GSRayTrace::OnActive()
{
  GSBase::OnActive();

  Shader* shader = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/raytrace");
  m_fspp.SetPostProcessShader(shader);
  m_fspp.InitFullScreenQuad();

  // Create texture encoding scene info
  int arraySize =  NUM_ROWS * NUM_PIXELS_PER_ROW;
  uint32* data = new uint32[arraySize];
  for (int i = 0; i < arraySize; i++)
  {
    data[i] = 0x00000000;
  }

  // Some data, just for testing
  data[0] = 0x00000000; // vertex0: (0,   0,   0)
  data[1] = 0x00000080; // vertex1: (0.5, 0,   0)
  data[2] = 0x00008080; // vertex1: (0.5, 0.5, 0)
 
  m_tex.Create((unsigned char*)data, NUM_PIXELS_PER_ROW, NUM_ROWS, 4); 

  delete[] data;

  m_tex.SetFilter(AmjuGL::AMJU_TEXTURE_NEAREST);
}

bool GSRayTrace::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSRayTrace::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
