#ifndef GS_RIGID_BODY_H
#define GS_RIGID_BODY_H

#include "RigidBody.h"
#include <Vec2.h>
#include <Singleton.h>
#include "GSBase.h"

namespace Amju
{
class GSRigidBody : public GSBase
{
public:
  static const char* NAME;

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
