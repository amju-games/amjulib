#ifndef GS_TODAY_H_INCLUDED
#define GS_TODAY_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSToday : public GSGui
{
  GSToday();
  friend class Singleton<GSToday>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSToday> TheGSToday;
} // namespace
#endif
