#include "GSTexture.h"
#include <AmjuGL.h>
#include <Vec2.h>
#include <DrawAABB.h>
#include <Texture.h>
#include <ResourceManager.h>

namespace Amju
{
static Texture* m_tex[2] = { 0, 0 };

GSTexture::GSTexture()
{
  //m_nextState=...
}

void GSTexture::Update()
{
  //GSBase::Update();

}

void GSTexture::Draw()
{
  GSBase::Draw();
  AmjuGL::SetClearColour(Colour(1, 0, 0, 1));

  m_tex[0]->UseThisTexture();
    
  //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  static AABB aabb(-1, 1, -1, 1, -1, 1);
  DrawSolidAABB(aabb);


  AmjuGL::Translate(3, 0, 0);
  m_tex[1]->UseThisTexture();
  DrawSolidAABB(aabb);
}

void GSTexture::Draw2d()
{
  return;

  // Draw a textured rectangle in screen space 
  Vec2f pos(0, 0);
  Vec2f size(0.1f, 0.1f);

  float z = 0.5f;
  AmjuGL::Tris tris;
  AmjuGL::Vert verts[4] = 
  {
    AmjuGL::Vert(pos.x + size.x, pos.y - size.y, z,   1, 0,   0, 1, 0), // x, y, z, u, v, nx, ny, nz  
    AmjuGL::Vert(pos.x + size.x, pos.y, z,   1, 1,   0, 1, 0),
    AmjuGL::Vert(pos.x, pos.y, z,   0, 1,   0, 1, 0),
    AmjuGL::Vert(pos.x, pos.y - size.y, z,   0, 0,   0, 1, 0)
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

  AmjuGL::DrawTriList(tris);

}

void GSTexture::OnActive()
{
  //GSBase::OnActive();

  static const char* TEX0 = "test.bmp";
  static const char* TEX1 = "bw.bmp";
  //static const char* TEX = "font2d/arial_512_16pt.bmpa";

  m_tex[0] = (Texture*)TheResourceManager::Instance()->GetRes(TEX0);
  m_tex[1] = (Texture*)TheResourceManager::Instance()->GetRes(TEX1);
}

bool GSTexture::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSTexture::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
