#ifndef GS_BASE_H
#define GS_BASE_H

#include <string>
#include <GameState.h>

namespace Amju
{
class GSBase : public GameState
{
public:
  GSBase();
  virtual void Update();

protected:
  float m_time;
  float m_maxTime;
  GameState* m_nextState;
};
}

#endif
