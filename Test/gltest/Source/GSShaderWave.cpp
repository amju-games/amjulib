#include "GSShaderWave.h"
#include <AmjuGL.h>
#include <Shader.h>
#include "Grid.h"
#include <Font.h>

namespace Amju
{
static Grid grid;
static Shader* shader = 0;

GSShaderWave::GSShaderWave()
{
  //m_nextState=...
}

void GSShaderWave::Update()
{
  //GSBase::Update();

}

void GSShaderWave::Draw()
{
  GSBase::Draw();

  AmjuGL::SetColour(1, 0, 0, 1);

  shader->Begin();

  static float shaderTime = 0;
  shaderTime += 0.01f;
  shader->Set("gTime", shaderTime);

  grid.Draw();
  shader->End();
}

void GSShaderWave::Draw2d()
{
}

void GSShaderWave::OnActive()
{
  grid.Build(100, 20.0f);
  shader = AmjuGL::LoadShader("Shaders/wavedemo");
}

} // namespace
