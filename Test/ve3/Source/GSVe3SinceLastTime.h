#pragma once

#include <Singleton.h>
#include "GSVe3Guestbook.h"
#include "Player.h"

namespace Amju 
{
class GSVe3SinceLastTime : public GSVe3Guestbook
{
  GSVe3SinceLastTime();
  friend class Singleton<GSVe3SinceLastTime>;

public:
  virtual void OnActive();
  virtual void OnDeactive();

protected:
  virtual void InitGB() override;
};
typedef Singleton<GSVe3SinceLastTime> TheGSVe3SinceLastTime;
} // namespace

