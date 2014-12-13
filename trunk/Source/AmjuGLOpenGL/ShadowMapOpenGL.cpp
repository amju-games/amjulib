#include "ShadowMapOpenGL.h"
#include "RenderToTextureOpenGL.h"

namespace Amju
{
bool ShadowMapOpenGL::Init()
{
//  m_renderToTexture = new RenderToTextureOpenGL;
  
  m_renderToTexture = dynamic_cast<RenderToTexture*>(
    AmjuGL::Create(RenderToTexture::DRAWABLE_TYPE_ID));
  
  m_renderToTexture->SetRenderFlags(RenderToTexture::AMJU_RENDER_DEPTH);
  m_renderToTexture->SetSize(1024, 1024);
  m_renderToTexture->Init();

  return true;
}

void ShadowMapOpenGL::Draw()
{
  // First pass: render depth into texture from light POV
  m_renderToTexture->Begin();
  AmjuGL::UseShader(0); // default shader, required for ES2 - or use a different shader

//  glClear(GL_DEPTH_BUFFER_BIT);
//  AmjuGL::UseShader(0); // default shader, required for ES2
    // Actually shader can be simpler, as only depth is required.

  // Render scene from light POV into texture. We only care about depth.
  // Set up frustum from light point of view
  // TODO
//  m_lightCam.Draw();
  m_cam.Draw(); // TEMP TEST - we know this points at teapot

//  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  m_drawFunc();
  m_renderToTexture->End();
/*
  // Second pass: use depth and other textures to render scene
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  
  m_renderToTexture->UseThisTexture();

  // Use shadow mapping (frag) shader  
  // TODO
  AmjuGL::UseShader(m_shader);

  m_cam.Draw(); 
  m_drawFunc();

  AmjuGL::UseShader(0); // default shader, required for ES2
*/
//  glDisable(GL_DEPTH_TEST);
  m_renderToTexture->DebugDraw(); 
//  glEnable(GL_DEPTH_TEST);
}

}
