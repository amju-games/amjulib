#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "EventListener.h"

namespace Amju
{
class GameState : public EventListener
{
public:
  virtual ~GameState() {}

  virtual void Update() = 0;
  virtual void Draw() = 0;
  virtual void Draw2d() = 0;
  virtual void OnActive();
  virtual void OnDeactive();

protected:
};
}

#endif
