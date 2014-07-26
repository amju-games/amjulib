#include <AmjuGL.h>
#include <Timer.h>

#ifdef WIN32
#include <gl/glew.h>
#include <GL/gl.h>
#endif

#ifdef MACOSX
#include <OpenGL/gl.h>
#endif

#include <Shader.h>
#include <Matrix.h>
#include <ResourceManager.h>
#include "Grid.h"
#include "GSTerrain.h"
#include "GSLighting.h"

namespace Amju
{
static Grid grid;

GSTerrain::GSTerrain()
{
  m_name = "Terrain";
  m_maxTime = 700.0f; 
}

void GSTerrain::Update()
{
  GSBase::Update();
}

void GSTerrain::Draw()
{
  AmjuGL::SetClearColour(Colour(0.5f, 1, 1, 1));

  /// NOT: GSBase::Draw();

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEARDIST = 1.0f; // NEAR and FAR are #defined in Windows, nice one.
  const float FARDIST = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEARDIST, FARDIST);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::LookAt(0, 2.4, 4.0,  0, 0, -10.0,  0, 1, 0);
  m_shader->Begin();


  // TODO AmjuGL SetActiveTexture() ....?
  // Or UseThisTexture() has tex unit ID ?
  glActiveTexture(GL_TEXTURE0);
  m_shader->Set("heightSampler", (AmjuGL::TextureHandle)0);
  m_heightmap->UseThisTexture();

  glActiveTexture(GL_TEXTURE1);
  m_shader->Set("diffuseSampler", (AmjuGL::TextureHandle)1);
  m_diffuse->UseThisTexture();

  glActiveTexture(GL_TEXTURE2);
  m_shader->Set("detailSampler", (AmjuGL::TextureHandle)2);
  m_detail->UseThisTexture();

  static float angle = 0;
  angle += TheTimer::Instance()->GetDt() * 4.0f;
  AmjuGL::RotateY(angle);

#ifdef AMJU_USE_ES2
  // Required for ES, not desktop OpenGL
  Matrix mv;
  mv.ModelView();
  Matrix proj;
  proj.Projection();
  Matrix mat = mv * proj;
  m_shader->Set("modelViewProjectionMatrix", mat);
#endif

  grid.Draw();
  m_shader->End();

  glActiveTexture(GL_TEXTURE0);
}

void GSTerrain::Draw2d()
{
  GSBase::Draw2d();
}

void GSTerrain::OnActive()
{
  m_heightmap = (Texture*)TheResourceManager::Instance()->GetRes("terrain/heightmap.png");
  Assert(m_heightmap);

  m_diffuse = (Texture*)TheResourceManager::Instance()->GetRes("terrain/diffuse.png");
  Assert(m_diffuse);

  m_detail = (Texture*)TheResourceManager::Instance()->GetRes("terrain/detail.png");
  Assert(m_detail);

  m_shader = AmjuGL::LoadShader("Shaders/" + GetShaderDir() + "/terrain");
  m_shader->Begin(); // so we find attrib var locations when we build tri list:
  grid.Build(26, 20.0f);
  m_shader->End();
}

} // namespace
