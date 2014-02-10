#ifndef TEST_STATE_H
#define TEST_STATE_H

#include "GameState.h"
#include "Singleton.h"

namespace Amju
{
class TestState : public GameState
{
private:
  TestState() {}
  friend class Singleton<TestState>;

public:
  virtual void OnActive();
  virtual void Update() {}
  virtual void Draw();
  virtual void Draw2d();

protected:
};

typedef Singleton<TestState> TheTestState;
}

#endif

