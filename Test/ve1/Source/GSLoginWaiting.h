#ifndef GS_LOGINWAITING_H_INCLUDED
#define GS_LOGINWAITING_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"

namespace Amju 
{
class GSLoginWaiting : public GSBase
{
  GSLoginWaiting();
  friend class Singleton<GSLoginWaiting>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSLoginWaiting> TheGSLoginWaiting;
} // namespace
#endif
