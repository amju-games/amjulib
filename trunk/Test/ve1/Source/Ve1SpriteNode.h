#ifndef VE1_SPRITE_NODE_H_INCLUDED
#define VE1_SPRITE_NODE_H_INCLUDED

#include "Ve1Character.h"
#include "Sprite.h"

namespace Amju
{
class Ve1SpriteNode : public Ve1Character
{
public:
  virtual void Draw();
  virtual void Update();

  void SetAnim(Anim anim);

  virtual void SetFromCharacterName(const std::string&);

private:
  Sprite m_sprite;
};
}

#endif

