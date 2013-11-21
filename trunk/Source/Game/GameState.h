#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "EventListener.h"

namespace Amju
{
class GameState : public EventListener
{
public:
  GameState() : m_prevState(nullptr) {}
  virtual ~GameState() {}

  virtual void Update() = 0;
  virtual void Draw() = 0;
  virtual void Draw2d() = 0;
  virtual void OnActive();
  virtual void OnDeactive();

  void SetPrevState(GameState* prev);
  GameState* GetPrevState();
  void GoBack();  // return to prev state - must have been set!

protected:
  GameState* m_prevState;
};
}

#endif
