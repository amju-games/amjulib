#pragma once

#include <Singleton.h>
#include "TradeGui.h"
#include "Ve1SpriteNode.h"
#include "Player.h"
#include "TradeType.h"

namespace Amju
{
class Player;

class GSVe3MakeTradeRequest : public TradeGui
{
  GSVe3MakeTradeRequest();
  friend class Singleton<GSVe3MakeTradeRequest>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  // Send trade request message to the other player
  void OnTradeSend();

private:
  Ve1SpriteNode m_spriteNode;
};
typedef Singleton<GSVe3MakeTradeRequest> TheGSVe3MakeTradeRequest;

}
