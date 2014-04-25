#pragma once

#include <Singleton.h>
#include "GSGui.h"
#include "Ve3ShowPlayer.h"

namespace Amju 
{
class GSVe3MyStats : public GSGui, public Ve3ShowPlayer
{
  GSVe3MyStats();
  friend class Singleton<GSVe3MyStats>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void RefreshGui();

protected:
};

typedef Singleton<GSVe3MyStats> TheGSVe3MyStats;
} // namespace

