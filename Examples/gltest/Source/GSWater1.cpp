#include <AmjuGL.h>
#include <Shader.h>
#include <Font.h>
#include <Matrix.h>
#include <Timer.h>
#include "Grid.h"
#include "GSWater1.h"
#include "GSLighting.h"
#include "simplexnoise.h"
#include "Tweakable.h"

namespace Amju
{
static float shaderTime = 0;

static float Water1HeightFunc(float x, float z)
{
  float a = raw_noise_2d(x * 0.2f + shaderTime, z * 0.2f);
  float b = raw_noise_2d(x * 0.2f, z * 0.2f + shaderTime);
  return 0.05f * (a + b);
}

GSWater1::GSWater1()
{
  m_name = "Water 1";
  m_description = "Simplex noise. Updating position and normals on the CPU.";
}
  
void GSWater1::CreateTweakMenu() 
{
  GSWaterBase::CreateTweakMenu();
}

void GSWater1::Update()
{
  GSWaterBase::Update();

  // Calc heights in software, not in shader: this is so we can recalc normals 
  //  and tangents. 
  // OR do it in the shader: we can get the heights of neighbour verts and calc
  //  normals in the shader. Tangents are easy for a grid.

  if (!m_paused) 
  {
    float dt = TheTimer::Instance()->GetDt();
    shaderTime += dt;
    m_gridWater.Rebuild(Water1HeightFunc); 
  }
}

void GSWater1::DrawScene()
{
  GSWaterBase::DrawScene();
}

void GSWater1::OnActive()
{
  GSWaterBase::OnActive();

  m_shaderWater = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/water1");
  m_shaderWater->Begin(); // so we find attrib var locations when we build tri list:
  m_gridWater.Build(m_gridSizeWater, 20.0f, Water1HeightFunc);
  
}

} // namespace
