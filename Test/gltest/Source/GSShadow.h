#ifndef GS_SHADOW_H_INCLUDED
#define GS_SHADOW_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"

namespace Amju 
{
class GSShadow : public GSBase
{
  GSShadow();
  friend class Singleton<GSShadow>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSShadow> TheGSShadow;
} // namespace
#endif
