#pragma once

#if defined (AMJU_IOS) || defined (ANDROID_NDK)

#include <DepthOfField.h>
#include <TriList.h>
#include "FullScreenPostProcessES2.h"

namespace Amju
{
class DepthOfFieldES2 : public DepthOfField, public FullScreenPostProcessES2
{
public:
  void Draw() override;
  bool Init() override;
  
private:

};
}

#endif

