#pragma once

#include <GameState.h>
#include <Singleton.h>

namespace Amju
{
class GSSimplest : public GameState
{
public:
  virtual void Draw() override;
  virtual void Draw2d() override {}
  virtual void Update() override {}
};

typedef Singleton<GSSimplest> TheGSSimplest;
}

