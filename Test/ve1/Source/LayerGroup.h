#ifndef LAYER_GROUP_H_INCLUDED
#define LAYER_GROUP_H_INCLUDED

#include "Sprite.h"

namespace Amju
{
// Selection of textures and colours available for one layer
class Layer
{
public:
  bool Load(File*);

private:
  typedef std::vector<PTexture> Textures;
  typedef std::vector<Colour> Colours;

  friend class LayerGroupManager;

  Textures m_textures;
  Colours m_colours;
  std::string m_name;
};

// Single instance where all layers are kept
class LayerGroupManager : public NonCopyable
{
private:
  LayerGroupManager();
  friend Singleton<LayerGroupManager>;

public:
  bool Load(File*);

  int GetNumTextures(int layer) const;
  int GetNumColours(int layer) const;

  Texture* GetTexture(int layer, int num) const;
  const Colour& GetColour(int layer, int num) const;

private:
  typedef std::vector<Layer> Layers;
  Layers m_layers;
};

typedef Singleton<LayerGroupManager> TheLayerGroupManager; 

// Has a vec of LayerGroup, and a currently active group.
// Within the currently active group, we can traverse back and forward
//  through the textures and colours, independently of each other.
// So we have a complete set of layers used for the Player, which we
//  can send to the server.
class LayerGroups
{
public:
  LayerGroups() : m_currentLayer(0) {}

  void NextLayer();
  void PrevLayer();
  int GetCurrentLayer() const;

  // Go to the next/prev texture for the current layer
  void NextTexture();
  void PrevTexture();
  void NextColour();
  void PrevColour();

  // Return num textures in the currently active layer
  int GetNumTextures() const;

  int GetNumColours() const;

  // Set layers in the given LayerSprite
  void SetSprite(LayerSprite*);

  // ? So we start off looking like the player ?
  void SetFromSprite(const LayerSprite&);

  void SendToServer();

private:
  int m_currentLayer;
  struct LayerSet
  {
    int m_currentTexture;
    int m_currentColour;
  };
  typedef std::vector<LayerSet> LayerSets;
  LayerSets m_layers;
};

}

#endif

