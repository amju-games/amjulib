#ifndef GS_PAUSED_H_INCLUDED
#define GS_PAUSED_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSPaused : public GSGui
{
  GSPaused();
  friend class Singleton<GSPaused>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
};
typedef Singleton<GSPaused> TheGSPaused;
} // namespace
#endif
