#pragma once

#include <Singleton.h>
#include "GSRenderTextureBase.h"

namespace Amju 
{
class GSRenderColourToTexture : public GSRenderTextureBase
{
  GSRenderColourToTexture();
  friend class Singleton<GSRenderColourToTexture>;
};
typedef Singleton<GSRenderColourToTexture> TheGSRenderColourToTexture;
} // namespace

