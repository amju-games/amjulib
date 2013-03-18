#include "GuiImage.h"
#include "ResourceManager.h"

namespace Amju
{
const char* GuiImage::NAME = "gui-image";

void GuiImage::Draw()
{
  if (!IsVisible())
  {
    return;
  }

  if (GetCombinedPos() != m_oldPos || GetSize() != m_oldSize)
  {
    BuildTris();
    m_oldPos = GetCombinedPos();
    m_oldSize = GetSize();
  }

  Assert(m_texture);
  m_texture->UseThisTexture();

  AmjuGL::DrawTriList(m_tris);
}

void GuiImage::BuildTris()
{
  Vec2f size = GetSize();
  Vec2f pos = GetCombinedPos();

  AmjuGL::Vert verts[4] = 
  {
    AmjuGL::Vert(pos.x + size.x, pos.y - size.y, 0,   1, 0,   0, 1, 0), // x, y, z, u, v, nx, ny, nz  
    AmjuGL::Vert(pos.x + size.x, pos.y, 0,   1, 1,   0, 1, 0),
    AmjuGL::Vert(pos.x, pos.y, 0,   0, 1,   0, 1, 0),
    AmjuGL::Vert(pos.x, pos.y - size.y, 0,   0, 0,   0, 1, 0)
  };

  m_tris.clear();
  m_tris.reserve(2);
  AmjuGL::Tri tri;
  tri.m_verts[0] = verts[0];
  tri.m_verts[1] = verts[1];
  tri.m_verts[2] = verts[2];
  m_tris.push_back(tri);

  tri.m_verts[0] = verts[0];
  tri.m_verts[1] = verts[2];
  tri.m_verts[2] = verts[3];
  m_tris.push_back(tri);
}

bool GuiImage::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }

  std::string s;
  if (!f->GetDataLine(&s))
  {
    Assert(0);
    return false;
  }

  m_texture = (Texture*)TheResourceManager::Instance()->GetRes(s);
  if (!m_texture)
  {
    return false;
  }

  return true;
}

void GuiImage::SetTexture(Texture* tex)
{
  m_texture = tex;
}

Texture* GuiImage::GetTexture()
{
  return m_texture;
}

}
