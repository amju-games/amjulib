#ifndef LAYER_GROUP_H_INCLUDED
#define LAYER_GROUP_H_INCLUDED

#include "Sprite.h"

namespace Amju
{
class LayerGroup
{
public:
  LayerGroup() : m_currentTexture(0), m_currentColour(0) {}
  bool Load(File*);

private:
  typedef std::vector<PTexture> Textures;
  typedef std::vector<Colour> Colours;

  Textures m_textures;
  Colours m_colours;
  std::string m_name;

  int m_currentTexture;
  int m_currentColour;

  friend class LayerGroups;
};

// Single instance where all layers are kept
class LayerGroupManager : public NonCopyable
{
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
  bool Load(File*);

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
  typedef std::vector<LayerGroup> Groups;
  Groups m_groups;

  int m_currentLayer;
};

}

#endif

