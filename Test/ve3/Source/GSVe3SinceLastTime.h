#pragma once

#include <Singleton.h>
#include "GSGui.h"
#include "Player.h"

namespace Amju 
{
class GSVe3SinceLastTime : public GSGui
{
  GSVe3SinceLastTime();
  friend class Singleton<GSVe3SinceLastTime>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void SetPlayer(Player*);

protected:
  RCPtr<Player> m_player;
};
typedef Singleton<GSVe3SinceLastTime> TheGSVe3SinceLastTime;
} // namespace

