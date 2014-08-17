#include <Screen.h>
#include <ReportError.h>
#include "BarrelDist.h"

namespace Amju
{
void BarrelDist::Draw()
{
  m_renderToTexture->Begin();
  m_drawFunc();
  m_renderToTexture->End();
  
  DrawFullScreenQuad();
}

bool BarrelDist::Init()
{
  Drawable* dr = AmjuGL::Create(RenderToTexture::DRAWABLE_TYPE_ID);
  Assert(dr);

  m_renderToTexture = dynamic_cast<RenderToTexture*>(dr);
 
  if (!m_renderToTexture)
  {
    std::cout << "Failed to create RenderToTexture in BarrelDist::Init()!\n";
    return false;
  }

std::cout << "Created render to texture OK in barrel dist\n"; 
  int w = Screen::X();
  int h = Screen::Y();
  m_renderToTexture->SetSize(w, h);

  m_renderToTexture->SetRenderFlags(RenderToTexture::AMJU_RENDER_COLOUR);

  if (!m_renderToTexture->Init())
  {
    ReportError("Failed to initialise render texture");
    return false;
  }

  // TODO Shader name member
  Shader* sh = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/barrel");
  if (!sh)
  {
    return false;
  }

std::cout << "Loaded shader for barrel distortion effect OK\n";

  sh->Set("diffuseSampler", (AmjuGL::TextureHandle)0);

  SetPostProcessShader(sh);

  if (!InitFullScreenQuad())
  {
std::cout << "Failed to init full screen quad.\n";
    return false;
  }

  return true;
}
}
