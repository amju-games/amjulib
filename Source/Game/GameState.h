#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "EventListener.h"

namespace Amju
{
class GameState : public EventListener
{
public:
  GameState();
  virtual ~GameState();

  virtual void Update() = 0;
  virtual void Draw() = 0;
  virtual void Draw2d() = 0;
 
  // Call these base class impls from subclass impls!
  virtual void OnActive();
  virtual void OnDeactive();

  // Called if game is paused, e.g. phone call
  virtual void OnPauseGame() {}

  void SetPrevState(GameState* prev);
  GameState* GetPrevState();
  void GoBack();  // return to prev state - must have been set!

  int GetEventPriority() const { return m_eventPriority; }

protected:
  GameState* m_prevState;
  int m_eventPriority; 
};
}

#endif
