#if defined (AMJU_IOS) || defined (ANDROID_NDK)

#include <Vec2.h>
#include <Screen.h>
#include "DepthOfFieldES2.h"
#include "RenderToTextureES2.h"

namespace Amju
{
void DepthOfFieldES2::Draw()
{
  m_renderToTexture.Begin();
  m_drawFunc();
  m_renderToTexture.End();
  
  DrawFullScreenQuad();
}

bool DepthOfFieldES2::Init()
{
  if (!InitFullScreenQuad())
  {
    return false;
  }
  
  return m_renderToTexture.Init(1024, 1024); // TODO - screen size?
}
}

#endif

