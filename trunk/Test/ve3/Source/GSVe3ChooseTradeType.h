#pragma once

#include "GSGui.h"
#include "Player.h"

namespace Amju
{
// Choose whether to give food/recv treasure or vice-versa.
// This is the first step in making a trade offer.
// Maybe make this a TradeGui, although at this point in the game
// you would only know the other player - none of the other info
// about the trade is known yet.

// This state will show diagram of the trade, so it's clearer to the player
// what's going on...

class GSVe3ChooseTradeType;
typedef Singleton<GSVe3ChooseTradeType> TheGSVe3ChooseTradeType;

class GSVe3ChooseTradeType : public GSGui
{
private:
  GSVe3ChooseTradeType();
  friend TheGSVe3ChooseTradeType;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  // Move to choose trade type
  void OnGiveFood();
  void OnGiveTreasure();

  // Set the player with whom we want to trade 
  void SetOtherPlayer(Player* p) { m_player = p; }

private:
  RCPtr<Player> m_player;
};
}
