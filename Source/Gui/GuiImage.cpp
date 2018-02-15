// Amjulib - cross platform game engine
// (c) Copyright Jason Colman 2000-2017

#include <AmjuFirst.h>
#include <GuiImage.h>
#include <ResourceManager.h>
#include <Screen.h>
#include <StringUtils.h>
#include <AmjuFinal.h>

namespace Amju
{
const char* GuiImage::NAME = "gui-image";

void GuiImage::Draw()
{
  if (!IsVisible())
  {
    return;
  }

  if (!m_triList)
  {
    BuildTris();
  }

  Assert(m_texture);
  m_texture->UseThisTexture();

  const Vec2f& pos = GetCombinedPos();
  const Vec2f& size = GetSize();

  AmjuGL::PushMatrix();
  AmjuGL::Translate(pos.x, pos.y, 0);
  AmjuGL::Scale(size.x, size.y, 1);
  AmjuGL::Draw(m_triList);
  AmjuGL::PopMatrix();
}

void GuiImage::BuildTris()
{
  AmjuGL::Vert verts[4] = 
  {
    AmjuGL::Vert(1, -1, 0,   1, 0,   0, 1, 0), // x, y, z, u, v, nx, ny, nz  
    AmjuGL::Vert(1,  0, 0,   1, 1,   0, 1, 0),
    AmjuGL::Vert(0,  0, 0,   0, 1,   0, 1, 0),
    AmjuGL::Vert(0, -1, 0,   0, 0,   0, 1, 0)
  };

  AmjuGL::Tris tris;
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

  // Check for extra attributes after the texture path
  Strings strs = Split(s, ',');
  Assert(!strs.empty());

  bool stretch = false;
  AmjuGL::TextureFilter tf = AmjuGL::AMJU_TEXTURE_NICE;

  int n = static_cast<int>(strs.size());
  for (int i = 1; i < n; i++)
  {
    if (strs[i] == "stretch")
    {
      stretch = true;
    }

    if (strs[i] == "nearest")
    {
      // use nearest filtering, not nicest
      tf = AmjuGL::AMJU_TEXTURE_NEAREST;
    }
  }

  m_texture = (Texture*)TheResourceManager::Instance()->GetRes(strs[0]);
  if (!m_texture)
  {
    return false;
  }

  // Adjust height so aspect ratio is that of the image, unless we want to stretch the image.
  if (!stretch)
  {
    float screenAspect = (float)Screen::X() / (float)Screen::Y();
    float imageAspectInv = (float)m_texture->GetHeight() / (float)m_texture->GetWidth();
    m_size.y = m_size.x * imageAspectInv * screenAspect;
  }

  m_texture->SetFilter(tf);

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
