#ifndef GS_VIEWOBJ_H_INCLUDED
#define GS_VIEWOBJ_H_INCLUDED

#include <Singleton.h>
#include <GameState.h>
#include <EventListener.h>

namespace Amju 
{
class MyEventListener;

class GSViewObj : public GameState
{
  GSViewObj();
  friend class Singleton<GSViewObj>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  bool OnCursorEvent(const CursorEvent&);
  bool OnMouseButtonEvent(const MouseButtonEvent&);
  bool OnKeyEvent(const KeyEvent&);

private:
  MyEventListener* m_eventListener;
};

class MyEventListener : public EventListener
{
public:
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  virtual bool OnKeyEvent(const KeyEvent&);
};

typedef Singleton<GSViewObj> TheGSViewObj;
} // namespace
#endif
