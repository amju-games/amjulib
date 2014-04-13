#pragma once

#include "GSGui.h"
#include "TradeType.h"
#include "Player.h"

namespace Amju
{
// Factor out common useful code for GUI classes which display trading msgs
class TradeGui : public GSGui
{
public:
  TradeGui();

  // Set msg ID, and get details about trade from msg.
  void SetMsgId(int msgId);

  // Set trade info for when this is not to be taken from an existing msg
  void SetTradeType(TradeType tt);
  void SetGiveRecv(int give, int recv);

  // Set the ID of the player on the other end of the Trade
  void SetOtherPlayer(int playerId);

  // Call when done, so we can sanity check next time
  void Reset();

protected:
  RCPtr<Player> m_otherTradePlayer; // the player with whom you want to trade
  int m_otherPlayerId; // ID of player on other end of trade

  int m_msgId;
  int m_give;
  int m_recv;
  TradeType m_tradeType;
};
}
