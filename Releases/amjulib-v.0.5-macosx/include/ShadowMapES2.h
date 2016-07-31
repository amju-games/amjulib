#pragma once

#if defined (AMJU_IOS) || defined (ANDROID_NDK) || defined(AMJU_USE_ES2)

 #include "ShadowMap.h"

namespace Amju
{
// Implement shadow map for OpenGL ES 2
class ShadowMapES2 : public ShadowMap
{
public:
  virtual bool Init();
  virtual void Draw();
};
}

#endif // if defined(AMJU_IOS) || defined(ANDROID_NDK)

