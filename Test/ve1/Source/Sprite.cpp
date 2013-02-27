#include <Timer.h>
#include <ReportError.h>
#include "Sprite.h"

namespace Amju
{
// Size of player sprites
static const float SIZE = 40.0f;

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
 
  AmjuGL::PushMatrix();

  AmjuGL::Tri t[2];
  m_seq.MakeTris(m_cellNum, size, t, pos.x, pos.y);
  AmjuGL::Tris tris;
  tris.reserve(2);
  tris.push_back(t[0]);
  tris.push_back(t[1]);

  AmjuGL::RotateX(-90.0f); // so x-y plane is x-z plane
  // Translate so centred on sprite
  static const float HSIZE = SIZE * 0.5f;
  AmjuGL::Translate(-HSIZE, -HSIZE, 0); 
  AmjuGL::DrawTriList(tris);  

  AmjuGL::PopMatrix();
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

bool Sprite::Load(const std::string& texFilename, int numCellsX, int numCellsY)
{
  // Cell shape is not square
  if (!m_seq.Load(texFilename, numCellsX, numCellsY, SIZE * 0.5f, SIZE))
  {
    ReportError("Failed to load Sprite sheet "  + texFilename);
    return false;
  }
  return true;
}

void LayerSprite::AddLayer(const SpriteLayer& layer)
{
  m_map[layer.z] = layer;
}

void LayerSprite::DrawLayers(const Vec2f& pos, float size)
{
  AmjuGL::SetTextureFilter(AmjuGL::AMJU_TEXTURE_NEAREST);

  AmjuGL::PushMatrix();

  AmjuGL::Tri t[2];
  m_seq.MakeTris(m_cellNum, size, t, pos.x, pos.y);
  AmjuGL::Tris tris;
  tris.reserve(2);
  tris.push_back(t[0]);
  tris.push_back(t[1]);

  AmjuGL::RotateX(-90.0f); // so x-y plane is x-z plane
  // Translate so centred on sprite
  static const float HSIZE = SIZE * 0.5f;
  AmjuGL::Translate(-HSIZE, -HSIZE, 0); 

  for (LayerMap::iterator it = m_map.begin(); it != m_map.end(); ++it)
  {
    const SpriteLayer& layer = it->second;
    Assert(layer.tex);
    layer.tex->UseThisTexture();
 
    PushColour();
    MultColour(layer.colour);
    AmjuGL::DrawTriList(tris);
    PopColour();  
  }
  AmjuGL::PopMatrix();
}

void LayerSprite::SetLayerTex(int layer, Texture* tex)
{
  m_map[layer].tex = tex;
}

void LayerSprite::SetLayerColour(int layer, const Colour& col)
{
  m_map[layer].colour = col;
}
}

