#ifndef GS_RIGID_BODY_H
#define GS_RIGID_BODY_H

#include <Vec2.h>
#include <Singleton.h>
#include "GSBase.h"

namespace Amju
{
class GSRigidBody : public GSBase
{
public:
  GSRigidBody();
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  virtual bool OnKeyEvent(const KeyEvent&);

private:
  Vec2f m_point; // cursor pos
  bool m_paused; 
};

typedef Singleton<GSRigidBody> TheGSRigidBody;
}

#endif
