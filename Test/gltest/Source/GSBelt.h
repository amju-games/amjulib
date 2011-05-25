#ifndef GS_BELT_H
#define GS_BELT_H

#include <GameState.h>
#include "SpringSystem.h"

namespace Amju
{
class GSBelt : public GameState
{
public:
  static const char* NAME;

  GSBelt();
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual void OnCursorEvent(const CursorEvent&);
  virtual void OnMouseButtonEvent(const MouseButtonEvent&);

private:
  void SetSimpleSystem();
  void Set1DSystem();
  void Set2DSystem();
  void Set2DSystem2();

private:
  SpringSystem m_ss;
  bool m_isMouseDown;
};
}

#endif
