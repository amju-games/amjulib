#include "RenderToTexture.h"

namespace Amju
{
RenderToTexture::RenderToTexture()
{
  m_width = 0;
  m_height = 0;
  m_vpX = 0;
  m_vpY = 0;
  m_vpW = -1;
  m_vpH = -1;
  m_renderFlags = AMJU_RENDER_COLOUR;
}

void RenderToTexture::SetRenderFlags(int renderFlags)
{
  m_renderFlags = renderFlags;
}

void RenderToTexture::SetSize(int w, int h)
{ 
  m_width = w;
  m_height = h;
}

void RenderToTexture::SetClearColour(const Colour& cc)
{
  m_clearCol = cc;
}

}

