#ifndef BLINK_CHARACTER_H
#define BLINK_CHARACTER_H

#include "Animated.h"
#include "Texture.h"

namespace Amju
{
class BlinkCharacter : public Animated
{
public:
  BlinkCharacter();
  virtual void Draw();
  virtual void Update();

  virtual void SetFromCharacterName(const std::string& characterName);

protected:

  // TODO don't call these, which are special case - specialise behaviour in 
  //  SetFromCharacterName()
  bool LoadTextures(const std::string& texture1, const std::string& texture2);

  void SetTex(Texture* t1, Texture* t2);

protected:
  // 2 textures for blinking 
  PTexture m_pTex[2];
  float m_blinkTime;
  // TODO Each character should blink eyes at different rate
};
}

#endif
