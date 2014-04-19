#pragma once

#include <Singleton.h>
#include "TradeGui.h"

namespace Amju
{
class GSVe3ShowTrade : public TradeGui
{
  GSVe3ShowTrade();
  friend class Singleton<GSVe3ShowTrade>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void OnTradeReject();
  void OnTradeAccept();
  void OnTradeCounterOffer();

private:
};
typedef Singleton<GSVe3ShowTrade> TheGSVe3ShowTrade;
}
