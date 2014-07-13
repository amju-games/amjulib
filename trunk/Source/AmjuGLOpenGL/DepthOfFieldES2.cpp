#include <Vec2.h>
#include <Screen.h>
#include "DepthOfFieldES2.h"
#include "RenderToTextureES2.h"

namespace Amju
{
static RenderToTextureES2 renderToTexture;

void DepthOfFieldES2::Draw()
{
  renderToTexture.Begin();  
  m_drawFunc();
  renderToTexture.End();
  
  renderToTexture.UseThisTexture();
  
  // TODO push/pop
  // Draw quad in screen space
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
 
  AmjuGL::SetColour(1, 1, 1); // argh you fucker
  m_triList->Draw();

  AmjuGL::UseTexture(0); // so we change texture
}

bool DepthOfFieldES2::Init()
{
  // Make a rectangle in screen space
  Vec2f pos(-1.0f, 1.0f);
  Vec2f size(2.0f, 2.0f);
  
  float z = 0.5f;
  AmjuGL::Tris tris;
  const float XMIN = 0;
  const float XMAX = 1;
  const float YMAX = 1;
  const float YMIN = 0;
  AmjuGL::Vert verts[4] =
  {
    AmjuGL::Vert(pos.x + size.x, pos.y - size.y, z,   XMAX, YMIN,   0, 1, 0),
    AmjuGL::Vert(pos.x + size.x, pos.y, z,   XMAX, YMAX,   0, 1, 0),
    AmjuGL::Vert(pos.x, pos.y, z,   XMIN, YMAX,   0, 1, 0),
    AmjuGL::Vert(pos.x, pos.y - size.y, z,   XMIN, YMIN,   0, 1, 0)
  };
  
  tris.clear();
  tris.reserve(2);
  AmjuGL::Tri tri;
  tri.m_verts[0] = verts[0];
  tri.m_verts[1] = verts[1];
  tri.m_verts[2] = verts[2];
  tris.push_back(tri);
  
  tri.m_verts[0] = verts[0];
  tri.m_verts[1] = verts[2];
  tri.m_verts[2] = verts[3];
  tris.push_back(tri);
  
  m_triList = MakeTriList(tris);
  
  return renderToTexture.Init(1024, 1024);
}

}

