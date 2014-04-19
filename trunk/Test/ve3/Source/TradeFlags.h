#pragma once

namespace Amju
{
enum TradeResponse
{
  TRADE_REPLY_NONE,
  TRADE_REPLY_ACCEPT,
  TRADE_REPLY_REJECT,
  TRADE_REPLY_COUNTER,

  TRADE_REPLY_MAX
};

// Functions for setting flags on trades, so you can't respond more than once!
void SetTradeResponse(int msgId, TradeResponse);

TradeResponse GetTradeResponse(int msgId);
}
