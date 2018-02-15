// Amjulib - cross platform game engine
// (c) Copyright Jason Colman 2000-2018

#include <AmjuHash.h>
#include <Lerp.h>
#include <LoadVec2.h>
#include <GuiSprite.h>
#include <StringUtils.h>

namespace Amju
{
GuiSprite::SpriteSheetMap GuiSprite::s_sprites;

const char* GuiSprite::NAME = "gui-sprite";

GuiSprite::~GuiSprite()
{
  RemoveThis();
}

void GuiSprite::Draw()
{
  // TODO just for now. 
  GuiImage::Draw();
}

void GuiSprite::Animate(float animValue)
{
  // Going forward, add 1 because max cell is part of the anim sequence.
  int d = 1;
  if (m_maxCell < m_minCell)
  {
    // TODO
    // TODO If maxCell < minCell, we should do d = -1 ?
    // Doesn't quite work.
  }
  int cell = static_cast<int>(Lerp(
    static_cast<float>(m_minCell), 
    static_cast<float>(m_maxCell + d), 
    animValue));

  SetCell(cell);
}

void GuiSprite::SetCell(int cell)
{
  if (cell != m_cell)
  {
    m_cell = cell;
    BuildTris();
  }
}

void GuiSprite::BuildTris()
{
  float du = 1.f / static_cast<float>(m_numCellsXY.x);
  float dv = 1.f / static_cast<float>(m_numCellsXY.y);
  float u = du * static_cast<float>(m_cell % m_numCellsXY.x);
  float v = dv * static_cast<float>(m_cell / m_numCellsXY.x); // x, not y

  AmjuGL::Vert verts[4] =
  {
    AmjuGL::Vert(1, -1, 0,   u + du, v - dv,   0, 1, 0), // x, y, z, u, v, nx, ny, nz  
    AmjuGL::Vert(1,  0, 0,   u + du, v,   0, 1, 0),
    AmjuGL::Vert(0,  0, 0,   u,      v,   0, 1, 0),
    AmjuGL::Vert(0, -1, 0,   u,      v - dv,   0, 1, 0)
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

bool GuiSprite::Load(File* f)
{
  if (!GuiImage::Load(f))
  {
    return false;
  }

  m_texHash = HashString(m_texture->GetFilename());

  // Get sprite sheet layout info
  if (!LoadVec2(f, &m_numCellsXY))
  {
    f->ReportError("Expected sprite sheet number of cells in x and y.");
    return false;
  }

  // Get current cell. We can specify 2 cell numbers here, in which case we 
  //  animate over the cell range.

  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected cell number (1 or 2) for sprite.");
    return false;
  }

  Strings strs = Split(s, ',');
  Assert(!strs.empty());
  m_cell = ToInt(strs[0]);
  m_minCell = m_cell;
  m_maxCell = m_cell;
  if (strs.size() == 2)
  {
    m_maxCell = ToInt(strs[1]);
  }

  AddThis();
  return true;
}

void GuiSprite::DrawAllSprites()
{
  for (auto& p : s_sprites)
  {
    auto& vec = p.second;
    // Update a trilist from the quad in each element of vec.
  }
}

void GuiSprite::AddThis()
{
  s_sprites[m_texHash].push_back(this);
}

void GuiSprite::RemoveThis()
{
  auto& vec = s_sprites[m_texHash];
  vec.erase(std::remove(vec.begin(), vec.end(), this), vec.end());
}
}
