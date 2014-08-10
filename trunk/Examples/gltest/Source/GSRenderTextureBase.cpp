#include <Teapot.h>
#include <AmjuGL.h>
#include "GSRenderTextureBase.h"
#include "GSDepthOfField.h"

namespace Amju
{
GSRenderTextureBase::GSRenderTextureBase()
{
}

void GSRenderTextureBase::Update()
{
  GSBase::Update();
}

void GSRenderTextureBase::Draw()
{
  RenderToTexture* rt = m_fs.GetRenderTarget();
  rt->Begin();
  AmjuGL::UseShader(0); // default shader, required for ES2 - or use a different shader
  DrawBunnyScene();
  rt->End();  
  
  m_fs.DrawFullScreenQuad();
  
  AmjuGL::UseShader(0);
  AmjuGL::UseTexture(0); // ?
  AmjuGL::SetClearColour(Colour(0, 0, 0.5f, 1));
}

void GSRenderTextureBase::Draw2d()
{
  GSBase::Draw2d();
}

void GSRenderTextureBase::OnActive()
{
  RenderToTexture* rt = dynamic_cast<RenderToTexture*>(
    AmjuGL::Create(RenderToTexture::DRAWABLE_TYPE_ID));

  rt->SetSize(2048, 2048);
  rt->SetRenderFlags(m_renderFlags);
  rt->Init();
  m_fs.SetRenderTarget(rt);

  Shader* sh = AmjuGL::LoadShader("Shaders/" + 
    AmjuGL::GetShaderDir() + "/" + m_shaderName);
  AmjuGL::UseShader(sh); // Required, for setting up vertex buffer

  m_fs.SetPostProcessShader(sh);

  m_fs.InitFullScreenQuad();
}

bool GSRenderTextureBase::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSRenderTextureBase::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
