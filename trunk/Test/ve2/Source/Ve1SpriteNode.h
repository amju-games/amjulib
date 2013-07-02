#ifndef VE1_SPRITE_NODE_H_INCLUDED
#define VE1_SPRITE_NODE_H_INCLUDED

#include "Ve1Character.h"
#include "Sprite.h"

namespace Amju
{
// Scene Node type for players - layers of clothes etc, with animated eyes.
// TODO Rename this type, to perhaps PlayerNode.
class Ve1SpriteNode : public Ve1Character
{
public:
  Ve1SpriteNode();

  virtual void Draw();
  virtual void Update();

  void SetAnim(Anim anim);
  void SetAnimRange(int a, int b);

  virtual void SetFromCharacterName(const std::string&);

  void SetLayerTex(int layer, int texIndex);
  void SetLayerColour(int layer, int colIndex);

  void SetScale(const Vec2f& scale);
  const Vec2f& GetScale() const;

  LayerSprite& GetSprite();

private:
  LayerSprite m_sprite;
  EyesSprite m_eyes;
  // Blinking
  float m_blinkTime;
  float m_maxBlinkTime;
  float m_gazeTime;
  Vec2f m_scale;
};
}

#endif

