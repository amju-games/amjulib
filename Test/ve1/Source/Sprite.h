#ifndef AMJU_SPRITE_H_INCLUDED
#define AMJU_SPRITE_H_INCLUDED

#include <TextureSequence.h>
#include <Vec2.h>
#include <Colour.h>

namespace Amju
{
class Sprite
{
public:
  Sprite();
  void SetCellRange(int minCell, int maxCell);
  void SetCellTime(float cellTime); // set period each cell displayed
  void SetCell(int cell);

  int GetCell() const;

  void Draw(const Vec2f& pos, float size);
  void Update();
  bool Load(const std::string& texFilename, int numCellsX, int numCellsY);

protected:
  TextureSequence m_seq;
  int m_minCellNum;
  int m_maxCellNum;
  int m_cellNum;
  float m_maxCellTime;
  float m_cellTime;
};

// Sprite composed of layers
struct SpriteLayer
{
  SpriteLayer() {}
  SpriteLayer(int z_, Texture* tex_, const Colour& colour_) : z(z_), tex(tex_), colour(colour_)
  {
  }

  int z;
  PTexture tex;
  Colour colour;
};

class LayerSprite : public Sprite
{
public:
  void AddLayer(const SpriteLayer&);
  void DrawLayers(const Vec2f& pos, float size);
  void SetLayerTex(int layer, Texture*);
  void SetLayerColour(int layer, const Colour&);

private:
  typedef std::map<float, SpriteLayer> LayerMap;
  LayerMap m_map;
};
}

#endif

