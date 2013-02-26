#ifndef GS_2DAVATARMOD_H_INCLUDED
#define GS_2DAVATARMOD_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GS2dAvatarMod : public GSGui
{
  GS2dAvatarMod();
  friend class Singleton<GS2dAvatarMod>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GS2dAvatarMod> TheGS2dAvatarMod;
} // namespace
#endif
