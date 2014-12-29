#if defined (AMJU_IOS) || defined (ANDROID_NDK) || defined(AMJU_USE_ES2)

#include "ShadowMapES2.h"

namespace Amju
{
bool ShadowMapES2::Init()
{
	return false;
}

void ShadowMapES2::Draw()
{
  m_drawFunc();
}

}
#endif // if defined(AMJU_IOS) || defined(ANDROID_NDK)

