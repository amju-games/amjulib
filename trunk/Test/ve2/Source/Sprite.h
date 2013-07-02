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
  bool Load(const std::string& texFilename, int numCellsX, int numCellsY, float cellSizeX, float cellSizeY);

  const Vec2f& GetSize() const;

protected:
  TextureSequence m_seq;
  int m_minCellNum;
  int m_maxCellNum;
  int m_cellNum;
  float m_maxCellTime;
  float m_cellTime;
  Vec2f m_size;
};

// Sprite composed of layers
struct SpriteLayer
{
  SpriteLayer() : z(0), visible(true), texIndex(0), colIndex(0) {}
  SpriteLayer(int z_, Texture* tex_, const Colour& colour_, int texIndex_, int colIndex_) : 
    z(z_), tex(tex_), colour(colour_), visible(true), texIndex(texIndex_), colIndex(colIndex_)
  {
  }

  int z; // layer num
  PTexture tex;
  Colour colour;
  bool visible;
  int texIndex; // index used to look up into LayerGroupManager
  int colIndex;
};

class LayerSprite : public Sprite
{
public:
  void AddLayer(const SpriteLayer&);
  void DrawLayers(const Vec2f& pos, float size);

  void SetLayerTex(int layer, int texIndex); // indexes looked up in TheLayerGroupManager
  void SetLayerColour(int layer, int colIndex);
  void SetLayerVis(int layer, bool vis);

  // Erase all layers
  void Clear();

protected:
  friend class LayerGroups;
  typedef std::map<int, SpriteLayer> LayerMap;
  LayerMap m_map;
};

// Eyes: special case
// Eyes are open or closed (sleeping or blinking).
// If open, eyes have background (fixed colour), and another layer showing eye gaze.
class EyesSprite : public LayerSprite
{
public:
  EyesSprite();
  void SetEyesOpen(bool open);
  void SetEyeColour(int colIndex); // indexes looked up in TheLayerGroupManager
  void SetEyeGazeTex(Texture* tex);

private:
  int m_gaze;
};
}

#endif

