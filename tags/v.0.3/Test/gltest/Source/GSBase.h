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
  virtual void Draw(); // Set up proj and mview matrices

protected:
  float m_time;
  float m_maxTime;
  GSBase* m_nextState;
};
}

#endif
