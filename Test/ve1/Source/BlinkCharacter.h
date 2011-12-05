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

  bool LoadTextures(const std::string& texture1, const std::string& texture2);

protected:
  // 2 textures for blinking 
  PTexture m_pTex[2];
  float m_blinkTime;
  // TODO Each character should blink eyes at different rate
};
}

#endif
