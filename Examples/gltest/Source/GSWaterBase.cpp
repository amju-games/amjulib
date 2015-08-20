#include <AmjuGL.h>
#include <Shader.h>
#include <Font.h>
#include <Matrix.h>
#include <Timer.h>
#include "Grid.h"
#include "GSWaterBase.h"
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

GSWaterBase::GSWaterBase()
{
  m_name = "Water 1";
  m_description = "Simplex noise. Updating position and normals on the CPU.";
  m_nextState = TheGSLighting::Instance();
  m_maxTime = 7.0f; 
  m_gridSizeWater = 20;
  m_gridSizeTerrain = 20;
  m_drawWater = true;
  m_drawTerrain = true;
  m_isWireframeWater = false;
  m_isWireframeTerrain = false;
}
  
void GSWaterBase::CreateTweakMenu() 
{
  GSBase::CreateTweakMenu();
  AddTweakable(m_tweaker, new TweakableBool("Draw water", &m_drawWater));
  AddTweakable(m_tweaker, new TweakableBool("Draw terrain", &m_drawTerrain));
  AddTweakable(m_tweaker, new TweakableBool("Water wireframe", &m_isWireframeWater));
  AddTweakable(m_tweaker, new TweakableBool("Terrain wireframe", &m_isWireframeTerrain));
}

void GSWaterBase::Update()
{
  GSBase::Update();

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

void GSWaterBase::DrawScene()
{
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));
  DrawHelp();

  static float angle = 0;
  angle += TheTimer::Instance()->GetDt() * 4.0f;
  AmjuGL::RotateY(angle);

  // Required for ES, and we use in desktop OpenGL shaders for consistency
  Matrix mv;
  mv.ModelView();
  Matrix proj;
  proj.Projection();
  Matrix mat = mv * proj;

  if (m_drawTerrain)
  {
    m_shaderTerrain->Begin();
    // Draw terrain
    m_shaderTerrain->Set("heightSampler", (AmjuGL::TextureHandle)0);
    m_heightmapTerrain->UseThisTexture(0);

    m_shaderTerrain->Set("diffuseSampler", (AmjuGL::TextureHandle)1);
    m_diffuseTerrain->UseThisTexture(1);

    m_shaderTerrain->Set("detailSampler", (AmjuGL::TextureHandle)2);
    m_detailTerrain->UseThisTexture(2);

    m_shaderTerrain->Set("modelViewProjectionMatrix", mat);

    AmjuGL::SetIsWireFrameMode(m_isWireframeTerrain);
    m_gridTerrain.Draw();
    m_shaderTerrain->End();
  }

  if (m_drawWater)
  {
    m_shaderWater->Begin();
    m_shaderWater->Set("modelViewProjectionMatrix", mat);
 
    AmjuGL::Vec3 eye(m_camera.m_pos.x, m_camera.m_pos.y, m_camera.m_pos.z);
    m_shaderWater->Set("eyePos", eye); 

//  m_shader->Set("gTime", shaderTime);

    AmjuGL::SetIsWireFrameMode(m_isWireframeWater);

// TODO switch
//    m_spheremap->UseThisTexture(0);

    m_shaderWater->Set("cubemapSampler", (AmjuGL::TextureHandle)0);
    m_cubemap->Draw();

    m_gridWater.Draw();
    m_shaderWater->End();
  }

  AmjuGL::SetIsWireFrameMode(false);

  AmjuGL::UseShader(nullptr);
  m_skybox->Draw();
}

void GSWaterBase::OnActive()
{
  GSBase::OnActive();

  m_heightmapTerrain = (Texture*)TheResourceManager::Instance()->GetRes("terrain/heightmap.png");
  Assert(m_heightmapTerrain);

  m_diffuseTerrain = (Texture*)TheResourceManager::Instance()->GetRes("terrain/diffuse.png");
  Assert(m_diffuseTerrain);

  m_detailTerrain = (Texture*)TheResourceManager::Instance()->GetRes("terrain/detail.png");
  Assert(m_detailTerrain);

  m_shaderTerrain = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/terrain");
  m_shaderTerrain->Begin(); // so we find attrib var locations when we build tri list:
  m_gridTerrain.Build(m_gridSizeTerrain, 20.0f);
  m_shaderTerrain->End();

  m_shaderWater = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/water1");
  m_shaderWater->Begin(); // so we find attrib var locations when we build tri list:
  m_gridWater.Build(m_gridSizeWater, 20.0f, Water1HeightFunc);
  
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
