#include <Vec2.h>
#include "FullScreenPostProcessES2.h"

namespace Amju
{
void FullScreenPostProcessES2::DrawFullScreenQuad()
{
  m_renderToTexture.UseThisTexture();
  
  // Use post-process shader
  AmjuGL::UseShader(m_postProcessShader);
  
  // TODO push/pop
  // Draw quad in screen space
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
 
  AmjuGL::SetColour(1, 1, 1); 
  m_triList->Draw();

  AmjuGL::UseTexture(0); // so we change texture
}

bool FullScreenPostProcessES2::InitFullScreenQuad()
{
  // Make a rectangle in screen space
  Vec2f pos(-1.0f, 1.0f);
  Vec2f size(2.0f, 2.0f);
  
  float z = 0.5f;
  AmjuGL::Tris tris;
  const float U_MIN = 0;
  const float U_MAX = 1;
  const float V_MAX = 1;
  const float V_MIN = 0;
  AmjuGL::Vert verts[4] =
  {
    AmjuGL::Vert(pos.x + size.x, pos.y - size.y, z,   U_MAX, V_MIN,   0, 1, 0),
    AmjuGL::Vert(pos.x + size.x, pos.y, z,   U_MAX, V_MAX,   0, 1, 0),
    AmjuGL::Vert(pos.x, pos.y, z,   U_MIN, V_MAX,   0, 1, 0),
    AmjuGL::Vert(pos.x, pos.y - size.y, z,   U_MIN, V_MIN,   0, 1, 0)
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
  
  return true;
}

}

