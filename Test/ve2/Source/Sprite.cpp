#include <AmjuFirst.h>
#include <Timer.h>
#include <ReportError.h>
#include "Sprite.h"
#include "LayerGroup.h"
#include <AmjuFinal.h>

namespace Amju
{
Sprite::Sprite()
{
  m_minCellNum = 0;
  m_maxCellNum = 0;
  m_cellNum = 0;
  m_maxCellTime = 0;
  m_cellTime = 0;
}

void Sprite::SetCellRange(int minCell, int maxCell)
{
  m_minCellNum = minCell;
  m_maxCellNum = maxCell;
}

void Sprite::SetCellTime(float cellTime)
{
  m_maxCellTime = cellTime;
  m_cellTime = 0; // ?
}

int Sprite::GetCell() const
{
  return m_cellNum;
}

void Sprite::SetCell(int cell)
{
  m_cellNum = cell;
}

void Sprite::Draw(const Vec2f& pos, float size)
{
  m_seq.Bind();
 
  //AmjuGL::PushMatrix();

  AmjuGL::Tri t[2];
  m_seq.MakeTris(m_cellNum, size, t, pos.x, pos.y);
  AmjuGL::Tris tris;
  tris.reserve(2);
  tris.push_back(t[0]);
  tris.push_back(t[1]);

  AmjuGL::DrawTriList(tris);  

  //AmjuGL::PopMatrix();
}

void Sprite::Update()
{
  float dt = TheTimer::Instance()->GetDt();
  m_cellTime += dt;
  if (m_cellTime > m_maxCellTime)
  {
    m_cellTime = 0;
    m_cellNum++;
    if (m_cellNum >= m_maxCellNum)
    {
      m_cellNum = m_minCellNum;
    }
  }
}

const Vec2f& Sprite::GetSize() const
{
  return m_size;
}

bool Sprite::Load(const std::string& texFilename, int numCellsX, int numCellsY, float cellSizeX, float cellSizeY)
{
  if (!m_seq.Load(texFilename, numCellsX, numCellsY, cellSizeX, cellSizeY))
  {
    ReportError("Failed to load Sprite sheet "  + texFilename);
    return false;
  }
  m_size = Vec2f(cellSizeX, cellSizeY);
  return true;
}

void LayerSprite::AddLayer(const SpriteLayer& layer)
{
  m_map[layer.z] = layer;
}

void LayerSprite::DrawLayers(const Vec2f& pos, float size)
{
  AmjuGL::Tri t[2];
  m_seq.MakeTris(m_cellNum, size, t, pos.x, pos.y);
  AmjuGL::Tris tris;
  tris.reserve(2);
  tris.push_back(t[0]);
  tris.push_back(t[1]);

  for (LayerMap::iterator it = m_map.begin(); it != m_map.end(); ++it)
  {
    const SpriteLayer& layer = it->second;
    if (!layer.visible)
    {
      continue;
    }

    Assert(layer.tex);
    layer.tex->UseThisTexture();
    AmjuGL::SetTextureFilter(AmjuGL::AMJU_TEXTURE_NEAREST);
 
    PushColour();
    MultColour(layer.colour);
    AmjuGL::DrawTriList(tris);
    PopColour();  
  }
}

void LayerSprite::SetLayerTex(int layer, int texIndex)
{
  Texture* t = TheLayerGroupManager::Instance()->GetTexture(layer, texIndex);
  Assert(t);

  m_map[layer].tex = t;
  m_map[layer].texIndex = texIndex;
}

void LayerSprite::SetLayerColour(int layer, int colIndex)
{
  const Colour& col = TheLayerGroupManager::Instance()->GetColour(colIndex);
  m_map[layer].colour = col;
  m_map[layer].colIndex = colIndex;
}

void LayerSprite::SetLayerVis(int layer, bool vis)
{
//  Assert(m_map.find(layer) != m_map.end());
  m_map[layer].visible = vis;
}

void LayerSprite::Clear()
{
  m_map.clear();
}
 
EyesSprite::EyesSprite() : m_gaze(0)
{
}

void EyesSprite::SetEyesOpen(bool open)
{
  if (open)
  {
    // closed eyes (zero layer) is invis.
    SetLayerVis(0, false);
    // Whites are vis.
    SetLayerVis(1, true);
    // Gaze dir is vis
    SetLayerVis(2, true);
  }
  else
  {
    // All except closed eyes layer is invis.
    SetLayerVis(0, true);
    SetLayerVis(1, false);
    SetLayerVis(2, false);
  }
}

void EyesSprite::SetEyeColour(int colIndex)
{
  SetLayerColour(2, colIndex);
}

void EyesSprite::SetEyeGazeTex(Texture* tex)
{
  m_map[2].tex = tex;
}

}

