#pragma once

#include <Singleton.h>
#include "GSRenderTextureBase.h"

namespace Amju 
{
class GSRenderDepthToTexture : public GSRenderTextureBase
{
  GSRenderDepthToTexture();
  friend class Singleton<GSRenderDepthToTexture>;
};
typedef Singleton<GSRenderDepthToTexture> TheGSRenderDepthToTexture;
} // namespace

