#pragma once

#include <GameState.h>
#include <Singleton.h>

namespace Amju
{
class GSLuaExample : public GameState
{
public:
  virtual void Draw() override;
  virtual void Draw2d() override;
  virtual void Update() override;
  virtual void OnActive() override;
};

typedef Singleton<GSSimplest> TheGSLuaExample;
}

