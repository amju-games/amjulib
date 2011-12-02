#ifndef GS_MAIN_H_INCLUDED
#define GS_MAIN_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"
#include <Vec2.h>

namespace Amju 
{
class GSMain : public GSBase
{
  GSMain();
  friend class Singleton<GSMain>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

private:
  void DoMoveRequest();

private:
  Vec2f m_mouseScreen;
  bool m_moveRequest;
};
typedef Singleton<GSMain> TheGSMain;
} // namespace
#endif
