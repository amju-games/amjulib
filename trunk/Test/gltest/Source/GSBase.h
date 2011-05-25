#ifndef GS_BASE_H
#define GS_BASE_H

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
  std::string m_nextStateName;
};
}

#endif
