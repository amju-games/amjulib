#ifndef GS_BARREL_H_INCLUDED
#define GS_BARREL_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"

namespace Amju 
{
class GSBarrel : public GSBase
{
  GSBarrel();
  friend class Singleton<GSBarrel>;

public:
  virtual void Update();
  virtual void DrawScene();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSBarrel> TheGSBarrel;
} // namespace
#endif
