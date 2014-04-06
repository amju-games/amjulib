#pragma once

#include <Singleton.h>
#include "GSGui.h"
//#include "Ve3ShowPlayer.h"
#include "Ve1SpriteNode.h"
#include "Player.h"

namespace Amju
{
class Player;

enum TradeType
{
  TRADE_NONE,
  TRADE_FOOD_FOR_TREASURE, 
  TRADE_TREASURE_FOR_FOOD,
};

class GSVe3MakeTradeRequest : public GSGui
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

  void SetPlayer(Player* p);

  void SetTradeType(TradeType tt);

private:
  RCPtr<Player> m_player; // the player with whom you want to trade
  Ve1SpriteNode m_spriteNode;
  TradeType m_tradeType;
};
typedef Singleton<GSVe3MakeTradeRequest> TheGSVe3MakeTradeRequest;

}
