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
static Grid grid;
static float shaderTime = 0;

static float Water1HeightFunc(float x, float z)
{
  float a = raw_noise_2d(x * 0.2f + shaderTime, z * 0.2f);
  float b = raw_noise_2d(x * 0.2f, z * 0.2f + shaderTime);
  return 0.3f * (a + b);
}

GSWater1::GSWater1()
{
  m_name = "Water 1";
  m_description = "Simplex noise. Updating position and normals on the CPU.";
  m_nextState = TheGSLighting::Instance();
  m_maxTime = 7.0f; 
}
  
void GSWater1::SetUpTweakMenu() 
{
  CreateTweakMenu();
  AddTweakable(m_tweaker, new TweakableFloat("Float test", nullptr, 0, 1));
}

void GSWater1::Update()
{
  GSBase::Update();

  // Calc heights in software, not in shader: this is so we can recalc normals 
  //  and tangents. 
  // TODO How to do this in shader???

  if (!m_paused) 
  {
    float dt = TheTimer::Instance()->GetDt();
    shaderTime += dt;
    grid.Rebuild(Water1HeightFunc); 
  }
}

void GSWater1::DrawScene()
{
  AmjuGL::SetClearColour(Colour(0.5f, 1, 1, 1));
  DrawHelp();

  m_shader->Begin();
  Matrix mv;
  mv.ModelView();
  Matrix proj;
  proj.Projection();
  Matrix mat = mv * proj;
  m_shader->Set("modelViewProjectionMatrix", mat);

//  m_shader->Set("gTime", shaderTime);

//  AmjuGL::SetIsWireFrameMode(true);

  m_sphereMap->UseThisTexture();

  grid.Draw();
  m_shader->End();

  AmjuGL::UseShader(nullptr);
  m_skybox->Draw();
}

void GSWater1::OnActive()
{
  GSBase::OnActive();

  m_shader = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/water1");
  m_shader->Begin(); // so we find attrib var locations when we build tri list:
  grid.Build(50, 20.0f, Water1HeightFunc);
  
  m_skybox = (ObjMesh*)TheResourceManager::Instance()->GetRes("skybox/skybox.obj");
  Assert(m_skybox);

  m_sphereMap = (Texture*)TheResourceManager::Instance()->GetRes("spheremap_bar.png");
  Assert(m_sphereMap);
}

} // namespace
