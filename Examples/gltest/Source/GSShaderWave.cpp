#include <AmjuGL.h>
#include <Shader.h>
#include <Font.h>
#include <Matrix.h>
#include "Grid.h"
#include "GSShaderWave.h"
#include "GSLighting.h"

namespace Amju
{
static Grid grid;

GSShaderWave::GSShaderWave()
{
  m_name = "Shader wave";
  m_nextState = TheGSLighting::Instance();
  m_maxTime = 7.0f; 
}

void GSShaderWave::Update()
{
  GSBase::Update();
}

void GSShaderWave::DrawScene()
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

  static float shaderTime = 0;
  shaderTime += 0.01f;
  m_shader->Set("gTime", shaderTime);

  grid.Draw();
  m_shader->End();
}

void GSShaderWave::OnActive()
{
  GSBase::OnActive();

  m_shader = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/wavedemo");
  m_shader->Begin(); // so we find attrib var locations when we build tri list:
  grid.Build(200, 20.0f);
}

} // namespace
