#include <Screen.h>
#include "DepthOfField.h"

namespace Amju
{
void DepthOfField::Draw()
{
  m_renderToTexture->Begin();
  m_drawFunc();
  m_renderToTexture->End();
  
  DrawFullScreenQuad();
}

bool DepthOfField::Init()
{
  if (!InitFullScreenQuad())
  {
    return false;
  }

  Drawable* dr = AmjuGL::Create(RenderToTexture::DRAWABLE_TYPE_ID);
  Assert(dr);

  m_renderToTexture = dynamic_cast<RenderToTexture*>(dr);
 
  if (!m_renderToTexture)
  {
    std::cout << "Failed to create RenderToTexture in DepthOfField::Init()!\n";
    return false;
  }

std::cout << "Created render to texture OK in depth of field\n"; 
//  int w = Screen::X();
//  int h = Screen::Y();
  m_renderToTexture->SetSize(512, 512); // TODO

  return m_renderToTexture->Init(); 
}
}
