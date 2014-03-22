#ifndef AMJU_TIMER_H
#define AMJU_TIMER_H

#include "Singleton.h"

namespace Amju
{
class Timer
{
private:
  Timer() {}
  friend class Singleton<Timer>;
public:
  void Update();
  float GetDt() const;
  float GetElapsedTime() const;
};

typedef Singleton<Timer> TheTimer;
}

#endif
