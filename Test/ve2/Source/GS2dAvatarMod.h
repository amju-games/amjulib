#ifndef GS_2DAVATARMOD_H_INCLUDED
#define GS_2DAVATARMOD_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"
#include "Ve1SpriteNode.h"
#include "LayerGroup.h"

namespace Amju 
{
class GS2dAvatarMod : public GSGui
{
  GS2dAvatarMod();
  friend class Singleton<GS2dAvatarMod>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void OnOk();
  void OnCancel();
  void OnSetColour(int colour);
  void OnSetLayer(int layer);
  void ActivateGroup(GuiElement* newGroup);
  void OnSetTexture(int texture);
  void OnScale(const Vec2f&);

private:
  //LayerSprite m_sprite; // shows the current settings
  Ve1SpriteNode m_spriteNode;

  LayerGroups m_layerGroups; // store settings for each layer

  GuiElement* m_oldGroup;
};
typedef Singleton<GS2dAvatarMod> TheGS2dAvatarMod;
} // namespace
#endif
