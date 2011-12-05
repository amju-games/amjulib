#ifndef GS_LOGIN_H_INCLUDED
#define GS_LOGIN_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"
#include "GSGui.h"

namespace Amju 
{
class GSLogin : public GSGui
{
  GSLogin();
  friend class Singleton<GSLogin>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSLogin> TheGSLogin;
} // namespace
#endif
