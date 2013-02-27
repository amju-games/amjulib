#ifndef GS_2DAVATARMOD_H_INCLUDED
#define GS_2DAVATARMOD_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"
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

// Has a vec of LayerGroup, and a currently active group.
// Within the currently active group, we can traverse back and forward
//  through the textures and colours, independently of each other.
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
  void SetFromSprite(const Sprite&);

private:
  typedef std::vector<LayerGroup> Groups;
  Groups m_groups;

  int m_currentLayer;
};

class GS2dAvatarMod : public GSGui
{
  GS2dAvatarMod();
  friend class Singleton<GS2dAvatarMod>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void OnNextLayer();
  void OnPrevLayer();
  void OnNextTex();
  void OnPrevTex();
  void OnNextCol();
  void OnPrevCol();

private:
  LayerSprite m_sprite; // shows the current settings

  LayerGroups m_layerGroups; 
};
typedef Singleton<GS2dAvatarMod> TheGS2dAvatarMod;
} // namespace
#endif
