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

  friend class LayerGroupManager;

  Textures m_textures;
  std::string m_name;
};

// Single instance where all layers are kept
class LayerGroupManager : public NonCopyable
{
private:
  LayerGroupManager();
  friend class Singleton<LayerGroupManager>;

public:
  bool Load(File*);

  int GetNumLayers() const;

  int GetNumTextures(int layer) const;
  Texture* GetTexture(int layer, int num) const;

  int GetNumColours() const;
  const Colour& GetColour(int num) const;

private:
  typedef std::vector<Layer> Layers;
  Layers m_layers;

  // Choice of colours is the same for all layers
  typedef std::vector<Colour> Colours;
  Colours m_colours;
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
  void SetCurrentLayer(int layer);
  int GetCurrentLayer() const;

  // Go to the next/prev texture for the current layer
  //void NextTexture();
  //void PrevTexture();
  void SetTexture(int);

  //void NextColour();
  //void PrevColour();

  // Set colour index for the currently active layer
  void SetColour(int);

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

