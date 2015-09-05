#include <AmjuGL.h>
#include "GSWater2.h"
#include "MySceneGraph.h"
#include "simplexnoise.h"
#include "Timer.h"
//#include "Tweakable.h"

namespace Amju
{
static float shaderTime = 0;

static float Water1HeightFunc(float x, float z)
{
  float a = raw_noise_2d(x * 0.2f + shaderTime, z * 0.2f);
  float b = raw_noise_2d(x * 0.2f, z * 0.2f + shaderTime);
  return 0.05f * (a + b);
}

GSWater2::GSWater2()
{
  m_grid = nullptr;
}

void GSWater2::Update()
{
  GSBase::Update();

    float dt = TheTimer::Instance()->GetDt();
    shaderTime += dt;
    m_grid->Rebuild(Water1HeightFunc); 
}

void GSWater2::DrawScene()
{
  GetSceneGraph()->GetCamera()->SetFromCamera(m_camera);
  GetSceneGraph()->Draw();
}

void GSWater2::DrawScene2d()
{
  GSBase::DrawScene2d();
}

void GSWater2::CreateTweakMenu()
{
  GSBase::CreateTweakMenu();
}

void GSWater2::OnActive()
{
  GSBase::OnActive();

  ResetSceneGraph();
  m_grid = new SceneNodeGrid;
  m_grid->Build(20, 10.0f);

  // Create material and add to node.
  Material* mat = new Material;
  m_grid->SetMaterial(mat);

  Shader* shaderWater = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/water1");
  shaderWater->Begin(); // ??? so we find attrib var locations when we build tri list:
  ////m_gridWater.Build(m_gridSizeWater, 20.0f, Water1HeightFunc);
  AmjuGL::Vec3 eye(m_camera.m_pos.x, m_camera.m_pos.y, m_camera.m_pos.z);
  shaderWater->Set("eyePos", eye); 

  mat->m_shader = shaderWater;

  Cubemap* cubemap = (Cubemap*)AmjuGL::Create(Cubemap::DRAWABLE_TYPE_ID);

  const std::string TEXTURE_NAMES[6] =
  {
    "skybox/Right.png",
    "skybox/Left.png",
    "skybox/Front.png",
    "skybox/Back.png",
    "skybox/Top.png",
    "skybox/Bottom.png",
  };
  cubemap->SetTextureNames(TEXTURE_NAMES);
  cubemap->SetTextureUnitId(0); // TODO Depends where texture unit is in Shader
  bool cubemapOk = cubemap->Init();
  Assert(cubemapOk);
  mat->m_cubemap = cubemap;

  ResetSceneGraph();
  GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->AddChild(m_grid);
}

bool GSWater2::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSWater2::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
