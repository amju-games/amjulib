#if defined(AMJU_IOS) || defined(ANDROID_NDK)

#include "ShadowMapES2.h"

namespace Amju
{
bool ShadowMapES2::Init()
{
}

void ShadowMapES2::Draw()
{
  m_drawFunc();
}

}
#endif // if defined(AMJU_IOS) || defined(ANDROID_NDK)

