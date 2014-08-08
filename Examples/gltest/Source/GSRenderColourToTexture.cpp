#include <Teapot.h>
#include <AmjuGL.h>
#include "GSRenderColourToTexture.h"
#include "GSRenderDepthToTexture.h"

namespace Amju
{
GSRenderColourToTexture::GSRenderColourToTexture()
{
  m_name = "Render colour to texture";
  m_nextState = TheGSRenderDepthToTexture::Instance();
  m_maxTime = 5.0f;
  m_shaderName = "rendertexture";
  m_renderFlags = RenderToTexture::AMJU_RENDER_COLOUR;
}
} // namespace
