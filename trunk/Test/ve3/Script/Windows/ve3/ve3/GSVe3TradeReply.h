#pragma once

#include <Singleton.h>
#include "GSGui.h"

namespace Amju
{
class GSVe3TradeReply : public GSGui
{
  GSVe3TradeReply();
  friend class Singleton<GSVe3TradeReply>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

private:

};
typedef Singleton<GSVe3TradeReply> TheGSVe3TradeReply;
}
