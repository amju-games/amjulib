#pragma once

#include <AmjuTypes.h>
#include <Texture.h>

namespace Amju
{
const int FIRE_SIZE = 32;
const int FIRE_ARRAY_SIZE = FIRE_SIZE * FIRE_SIZE;

class FireTexture
{
public:
  FireTexture();
  bool Init();
  void Update();
  Texture* GetTexture() { return &m_tex; }
 
protected:
  Texture m_tex;
 
  uint8 src[FIRE_ARRAY_SIZE];
  uint8 dst[FIRE_ARRAY_SIZE];
};
}

