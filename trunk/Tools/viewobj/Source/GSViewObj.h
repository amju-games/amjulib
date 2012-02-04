#ifndef GS_VIEWOBJ_H_INCLUDED
#define GS_VIEWOBJ_H_INCLUDED

#include <Singleton.h>
#include "GameState.h"

namespace Amju 
{
class GSViewObj : public GameState
{
  GSViewObj();
  friend class Singleton<GSViewObj>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  virtual bool OnKeyEvent(const KeyEvent&);
};
typedef Singleton<GSViewObj> TheGSViewObj;
} // namespace
#endif
