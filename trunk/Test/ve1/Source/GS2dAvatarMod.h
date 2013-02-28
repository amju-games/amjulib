#ifndef GS_2DAVATARMOD_H_INCLUDED
#define GS_2DAVATARMOD_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"
#include "Sprite.h"
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

  void OnNextLayer();
  void OnPrevLayer();
  void OnNextTex();
  void OnPrevTex();
  void OnNextCol();
  void OnPrevCol();
  void OnOk();
  void OnCancel();

private:
  LayerSprite m_sprite; // shows the current settings

  LayerGroups m_layerGroups; 
};
typedef Singleton<GS2dAvatarMod> TheGS2dAvatarMod;
} // namespace
#endif
