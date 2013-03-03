#ifndef GS_LOGOUT_H_INCLUDED
#define GS_LOGOUT_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSLogout : public GSGui
{
  GSLogout();
  friend class Singleton<GSLogout>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSLogout> TheGSLogout;
} // namespace
#endif
