#ifndef GS_RIGID_BODY_H
#define GS_RIGID_BODY_H

#include "RigidBody.h"
#include <Vec2.h>
#include <Singleton.h>
#include <GameState.h>

namespace Amju
{
class GSRigidBody : public GameState
{
public:
  GSRigidBody();
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

private:
  PRigidBody m_rb;
  Vec2f m_point; // cursor pos
};

typedef Singleton<GSRigidBody> TheGSRigidBody;
}

#endif
