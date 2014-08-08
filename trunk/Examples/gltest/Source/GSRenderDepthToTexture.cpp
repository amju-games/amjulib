#include <Teapot.h>
#include <AmjuGL.h>
#include "GSRenderDepthToTexture.h"
#include "GSDepthOfField.h"

namespace Amju
{
GSRenderDepthToTexture::GSRenderDepthToTexture()
{
  m_name = "Render depth to texture";
  m_nextState = TheGSDepthOfField::Instance();
  m_maxTime = 20.0f;
  m_shaderName = "rendertexture";
  m_renderFlags = RenderToTexture::AMJU_RENDER_DEPTH;
}
} // namespace
