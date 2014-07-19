#pragma once

#include <TriList.h>
#include "RenderToTextureES2.h"

namespace Amju
{
class FullScreenPostProcessES2
{
public:
  void DrawFullScreenQuad();
  bool InitFullScreenQuad();

  RenderToTextureES2* GetRenderTarget() { return &m_renderToTexture; }
  
protected:
  // Full screen quad 
  RCPtr<TriListStatic> m_triList;

  RenderToTextureES2 m_renderToTexture;
  
  Shader* m_postProcessShader;
};
}

