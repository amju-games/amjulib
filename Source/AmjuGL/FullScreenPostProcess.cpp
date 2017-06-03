#include <Vec2.h>
#include <AmjuGL.h>
#include "FullScreenPostProcess.h"

namespace Amju
{
FullScreenPostProcess::FullScreenPostProcess()
{
  m_pos = Vec2f(-1.0f, 1.0f);
  m_size = Vec2f(2.0f, 2.0f);
}
  
void FullScreenPostProcess::DrawFullScreenQuad()
{
  Assert(m_triList);
  //Assert(m_postProcessShader);
  //Assert(m_renderToTexture);
  
  // Use post-process shader
  if (m_postProcessShader)
  {
    AmjuGL::UseShader(m_postProcessShader);
  }
  
  if (m_renderToTexture)
  {
    m_renderToTexture->UseThisTexture();
  }
  
  // TODO push/pop
  // Draw quad in screen space
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
 
  AmjuGL::SetColour(1, 1, 1); 
  m_triList->Draw();

  if (m_renderToTexture)
  {
    AmjuGL::UseTexture(0); // so we change texture
  }
}

void FullScreenPostProcess::SetRect(const Vec2f& pos, const Vec2f& size)
{
  m_pos = pos;
  m_size = size;
}
  
bool FullScreenPostProcess::InitFullScreenQuad()
{
  // To set up attrib variables ????
//  Assert(m_postProcessShader);
//  AmjuGL::UseShader(m_postProcessShader);

  // Make a rectangle in screen space
  float z = 0.5f;
  AmjuGL::Tris tris;
  const float U_MIN = 0;
  const float U_MAX = 1;
  const float V_MAX = 1;
  const float V_MIN = 0;
  AmjuGL::Vert verts[4] =
  {
    AmjuGL::Vert(m_pos.x + m_size.x, m_pos.y - m_size.y, z,   U_MAX, V_MIN,   0, 1, 0),
    AmjuGL::Vert(m_pos.x + m_size.x, m_pos.y,            z,   U_MAX, V_MAX,   0, 1, 0),
    AmjuGL::Vert(m_pos.x,            m_pos.y,            z,   U_MIN, V_MAX,   0, 1, 0),
    AmjuGL::Vert(m_pos.x,            m_pos.y - m_size.y, z,   U_MIN, V_MIN,   0, 1, 0)
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

