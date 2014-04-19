#ifndef GS_BELT_H
#define GS_BELT_H

#include "GSBase.h"
#include <Singleton.h>
#include "SpringSystem.h"

namespace Amju
{
class GSBelt : public GSBase
{
  GSBelt();
  friend class Singleton<GSBelt>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

private:
  void SetSimpleSystem();
  void Set1DSystem();
  void Set2DSystem();
  void Set2DSystem2();

private:
  SpringSystem m_ss;
  bool m_isMouseDown;
};

typedef Singleton<GSBelt> TheGSBelt;
}

#endif
