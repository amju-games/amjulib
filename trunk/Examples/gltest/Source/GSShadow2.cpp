#include <Teapot.h>
#include <AmjuGL.h>
#include <Camera.h>
#include <Matrix.h>
#include "GSShadow2.h"
#include "GSDepthOfField.h"

namespace Amju
{
GSShadow2::GSShadow2()
{
  m_name = "Shadow 2";
  m_nextState = TheGSDepthOfField::Instance();
  m_maxTime = 500.0f;
  m_shaderName = "rendertexture";
  m_renderFlags = RenderToTexture::AMJU_RENDER_DEPTH;
}

void GSShadow2::OnActive()
{
  GSRenderTextureBase::OnActive();

  m_secondPassShader = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/shadowpass2");
  Assert(m_secondPassShader);

  //m_secondPassShader->Begin(); // so we find attrib var locations when we build tri list:
}

void GSShadow2::Draw()
{
  Camera lightCam;
  lightCam.SetPos(Vec3f(10, 10, 0));
  lightCam.SetDir(Vec3f(-1, -1, 0));
  lightCam.SetNearFar(1.0f, 50.0f);

  Camera viewCam;
  viewCam.SetPos(Vec3f(0, 5, 10));
  viewCam.SetDir(Vec3f(0, -4, -10));
  viewCam.SetNearFar(1.0f, 50.0f);

/*
  RenderToTexture* rt = m_fs.GetRenderTarget();
  rt->Begin();
  AmjuGL::UseShader(0); // default shader, required for ES2 - or use a different shader

  lightCam.Draw();
  DrawBunnyScene();
  rt->End();  

  // Get modelview * projection matrix from this pass, to give to 2nd pass
  //  frag shader
  Matrix mview;
  Matrix proj;
  mview.ModelView();
  proj.Projection();
  
 
  AmjuGL::UseShader(m_secondPassShader);

  AmjuGL::UseTexture(0); // ?
  AmjuGL::SetClearColour(Colour(0, 0, 0.5f, 1));

  viewCam.Draw(); // OK, openGL shader can get gl_ModelViewProjectionMatrix etc
  DrawBunnyScene();

  AmjuGL::UseShader(0);


  rt->DebugDraw(); // TODO TEMP TEST

  AmjuGL::UseShader(0);
*/

  viewCam.Draw();
  AmjuGL::UseShader(m_secondPassShader);
  DrawBunnyScene();
  AmjuGL::UseShader(0);
}
} // namespace
