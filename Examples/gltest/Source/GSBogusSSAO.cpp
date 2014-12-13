#include "GSBogusSSAO.h"
#include "GSShadow.h"

namespace Amju
{
GSBogusSSAO::GSBogusSSAO()
{
  m_name = "Bogus SSAO";
  m_nextState = TheGSShadow::Instance();
  m_maxTime = 5.0f;
  m_shaderName = "bogus_ssao";
  m_renderFlags = RenderToTexture::AMJU_RENDER_DEPTH;
}
} // namespace
