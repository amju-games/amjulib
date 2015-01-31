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
  return 0.05f * (a + b);
}

GSWater1::GSWater1()
{
  m_name = "Water 1";
  m_description = "Simplex noise. Updating position and normals on the CPU.";
  m_nextState = TheGSLighting::Instance();
  m_maxTime = 7.0f; 
  m_isWireframe = false;
}
  
void GSWater1::SetUpTweakMenu() 
{
  CreateTweakMenu();
  //AddTweakable(m_tweaker, new TweakableFloat("Float test", nullptr, 0, 1));
  AddTweakable(m_tweaker, new TweakableBool("Stereo", &s_isStereo));
  AddTweakable(m_tweaker, new TweakableBool("Wireframe", &m_isWireframe));
  AddTweakable(m_tweaker, new TweakableBool("Mouse look", &s_mouseLook));
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
 
  AmjuGL::Vec3 eye(m_camera.m_pos.x, m_camera.m_pos.y, m_camera.m_pos.z);
  m_shader->Set("eyePos", eye); 

//  m_shader->Set("gTime", shaderTime);

  AmjuGL::SetIsWireFrameMode(m_isWireframe);

  m_spheremap->UseThisTexture();

  grid.Draw();
  m_shader->End();

  AmjuGL::SetIsWireFrameMode(false);

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

  m_spheremap = (Texture*)TheResourceManager::Instance()->GetRes("spheremap_bar.png");
  Assert(m_spheremap);

  m_cubemap = (Cubemap*)AmjuGL::Create(Cubemap::DRAWABLE_TYPE_ID);

  // Cubemap textures:
  // +x (right)
  // -x (left)
  // +z (front)
  // -z (back)
  // +y (top)
  // -y (bottom)
  const std::string TEXTURE_NAMES[6] = 
  {
    "skybox/Right.png",
    "skybox/Left.png",
    "skybox/Front.png",
    "skybox/Back.png",
    "skybox/Top.png",
    "skybox/Bottom.png",
  };
  m_cubemap->SetTextureNames(TEXTURE_NAMES);
  m_cubemap->SetTextureUnitId(0); // TODO Depends where texture unit is in Shader
  bool cubemapOk = m_cubemap->Init();
  Assert(cubemapOk);
}

} // namespace
