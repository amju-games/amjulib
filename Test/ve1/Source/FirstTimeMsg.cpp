#include <Localise.h>
#include "FirstTimeMsg.h"
#include "LurkMsg.h"
#include "LocalPlayer.h"
#include "Player.h"
#include "ObjectUpdater.h"
#include "Ve1OnlineReqManager.h"

namespace Amju
{
bool FirstTimeMsg(const std::string& text, MsgNum msgNumber)
{
  std::string key = "shown_msg_" + ToString(msgNumber);
  if (GetLocalPlayer()->Exists(key))
  {
    // Already shown this msg
    return false;
  } 

  // Set shown flag
  TheObjectUpdater::Instance()->SendUpdateReq(GetLocalPlayer()->GetId(), key, "y");

  GetLocalPlayer()->SetKeyVal(key, "y");
  // TODO set shown flag when msg closed.

  // Show this msg
  LurkMsg lm(Lookup(text), Colour(1, 1, 1, 1), Colour(0, 0, 0, 1), AMJU_CENTRE); 
  TheLurker::Instance()->Queue(lm);

  return true;
}

bool FirstTimeMsgThisSession(const std::string& text, MsgNum msgNumber)
{
  typedef std::pair<int, MsgNum> PlayerMsg;
  typedef std::set<PlayerMsg> Msgs;

  static Msgs msgs;
  int session = ToInt(TheVe1ReqManager::Instance()->GetSessionId());
  PlayerMsg pm(session, msgNumber);
  if (msgs.count(pm) != 0)
  {
    return false;
  }   
  msgs.insert(pm);

  // Show this msg
  LurkMsg lm(Lookup(text), Colour(1, 1, 1, 1), Colour(0, 0, 0, 1), AMJU_CENTRE); 
  TheLurker::Instance()->Queue(lm);

  return true;
}

}

