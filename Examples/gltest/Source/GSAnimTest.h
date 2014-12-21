#ifndef GS_ANIMTEST_H_INCLUDED
#define GS_ANIMTEST_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"

namespace Amju 
{
class GSAnimTest : public GSBase
{
  GSAnimTest();
  friend class Singleton<GSAnimTest>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSAnimTest> TheGSAnimTest;
} // namespace
#endif
