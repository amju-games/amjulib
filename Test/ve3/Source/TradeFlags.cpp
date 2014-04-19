#include <StringUtils.h>
#include "TradeFlags.h"
#include "LocalPlayer.h"
#include "ObjectUpdater.h"

namespace Amju
{
static const char* TRADE_STRING[] = 
{
  "none",
  "accept",
  "reject",
  "counter"
};
  
  
static std::string MakeKey(int msgId)
{
  return "trade_" + ToString(msgId);
}

void SetTradeResponse(int msgId, TradeResponse tr)
{
  // Set key/val for local player
  if (GetTradeResponse(msgId) != TRADE_REPLY_NONE)
  {
    std::cout << "Already responded to this trade!?!\n";
    return;
  }

  int lpid = GetLocalPlayerId();
  std::string val = TRADE_STRING[(int)tr];
  std::string key = MakeKey(msgId);
  TheObjectUpdater::Instance()->SendUpdateReq(lpid, key, val);

  // TODO Immediately set in local player? Is this a good idea?
  GetLocalPlayer()->SetKeyVal(key, val);
}

TradeResponse GetTradeResponse(int msgId)
{
  // Create key, look up for local player
  Player* p = GetLocalPlayer();
  if (p)
  {
    std::string key = MakeKey(msgId);
    if (p->Exists(key))
    {
       std::string val = p->GetVal(key);
       for (int i = (int)TRADE_REPLY_ACCEPT; i < (int)TRADE_REPLY_MAX; i++)
       {
         if (val == TRADE_STRING[i])
         {
           return (TradeResponse)i;
         }
       }
    }
  }
  return TRADE_REPLY_NONE;
}

}
