#include "GSBogusSSAO.h"
#include "GSRenderColourToTexture.h"

namespace Amju
{
GSBogusSSAO::GSBogusSSAO()
{
  m_name = "Bogus SSAO";
  m_nextState = TheGSRenderColourToTexture::Instance();
  m_maxTime = 200.0f;
  m_shaderName = "bogus_ssao";
  m_renderFlags = RenderToTexture::AMJU_RENDER_DEPTH;
}
} // namespace
