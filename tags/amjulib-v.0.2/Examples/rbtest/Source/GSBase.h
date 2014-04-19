#ifndef GS_BASE_H
#define GS_BASE_H

#include <string>
#include <GameState.h>
#include <EventListener.h>

namespace Amju
{
class GSBase;

class GSBase : public GameState
{
public:
  GSBase();
  virtual void Update();
  virtual void Draw(); // Set up proj and mview matrices

  // Drag to rotate etc
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  virtual bool OnKeyEvent(const KeyEvent&);

protected:
  float m_time;
  float m_maxTime;
  GSBase* m_nextState;

 // RCPtr<GameStateListener> m_listener;
};
}

#endif
