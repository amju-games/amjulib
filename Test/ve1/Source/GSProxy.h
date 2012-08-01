#ifndef GS_PROXY_H_INCLUDED
#define GS_PROXY_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSProxy : public GSGui
{
  GSProxy();
  friend class Singleton<GSProxy>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSProxy> TheGSProxy;
} // namespace
#endif
