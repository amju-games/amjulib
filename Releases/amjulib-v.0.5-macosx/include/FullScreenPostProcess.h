#pragma once

#include <TriList.h>
#include <Shader.h>
#include <Vec2.h>
#include "RenderToTexture.h"

namespace Amju
{
// (Mixin?) class for post process effects
class FullScreenPostProcess
{
public:
  FullScreenPostProcess();
  
  void DrawFullScreenQuad();
  bool InitFullScreenQuad();

  RenderToTexture* GetRenderTarget() { return m_renderToTexture; }
  void SetRenderTarget(RenderToTexture* rt) { m_renderToTexture = rt; }

  Shader* GetPostProcessShader() { return m_postProcessShader; }
  void SetPostProcessShader(Shader* sh) { m_postProcessShader = sh; }
  
  void SetRect(const Vec2f& pos, const Vec2f& size);
  
protected:
  // Full screen quad 
  RCPtr<TriListStatic> m_triList;

  RCPtr<RenderToTexture> m_renderToTexture;
  
  RCPtr<Shader> m_postProcessShader;
  
  Vec2f m_pos; // default: (-1, 1)
  Vec2f m_size; // default: (2, 2)
};
}

