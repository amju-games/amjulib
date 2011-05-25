#ifndef GS_RIGID_BODY_H
#define GS_RIGID_BODY_H

#include <GameState.h>
#include "RigidBody.h"
#include <Vec2.h>

namespace Amju
{
class GSRigidBody : public GameState
{
public:
  static const char* NAME;

  GSRigidBody();
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual void OnCursorEvent(const CursorEvent&);
  virtual void OnMouseButtonEvent(const MouseButtonEvent&);

private:
  PRigidBody m_rb;
  Vec2f m_point; // cursor pos
};
}

#endif
