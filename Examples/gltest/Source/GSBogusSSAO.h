#ifndef GS_BOGUSSSAO_H_INCLUDED
#define GS_BOGUSSSAO_H_INCLUDED

#include <Singleton.h>
#include <FullScreenPostProcess.h>
#include "GSRenderTextureBase.h"

namespace Amju 
{
class GSBogusSSAO : public GSRenderTextureBase
{
  GSBogusSSAO();
  friend class Singleton<GSBogusSSAO>;
};
typedef Singleton<GSBogusSSAO> TheGSBogusSSAO;
} // namespace
#endif
