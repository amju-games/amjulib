#include <Teapot.h>
#include <AmjuGL.h>
#include "GSRenderTextureBase.h"
#include "GSDepthOfField.h"

namespace Amju
{
GSRenderTextureBase::GSRenderTextureBase()
{
  m_name = "Render colour to texture";
  m_nextState = TheGSDepthOfField::Instance();
  m_maxTime = 5.0f;
}

void GSRenderTextureBase::Update()
{
  GSBase::Update();
}

void GSRenderTextureBase::Draw()
{
  RenderToTexture* rt = m_fs.GetRenderTarget();
  rt->Begin();
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

  rt->SetSize(1024, 1024); 
  rt->SetRenderFlags(m_renderFlags);
  rt->Init();
  m_fs.SetRenderTarget(rt);

  Shader* sh = AmjuGL::LoadShader("Shaders/" + 
    AmjuGL::GetShaderDir() + "/" + m_shaderName);

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
