#include <Localise.h>
#include <Game.h>
#include "FirstTimeMsg.h"
#include "LurkMsg.h"
#include "LocalPlayer.h"
#include "Player.h"
#include "ObjectUpdater.h"
#include "Ve1OnlineReqManager.h"
#include "GSStory.h"
#include "GameConsts.h"

namespace Amju
{
static void ShowText(const std::string& text, bool storyMode)
{
  if (storyMode)
  {
    TheGSStory::Instance()->SetText(text);
    TheGSStory::Instance()->SetPrevState(TheGame::Instance()->GetState());
    TheGame::Instance()->SetCurrentState(TheGSStory::Instance());
  }
  else
  {
    // Show this msg
    LurkMsg lm(Lookup(text), LURK_FG, LURK_BG, AMJU_CENTRE); 
    TheLurker::Instance()->Queue(lm);
  }
}

bool FirstTimeMsg(const std::string& text, MsgNum msgNumber, bool storyMode)
{
  Player* p = GetLocalPlayer();
  if (!p)
  {
    std::cout << "No player, can't show msg!\n";
    Assert(0);
    return false;
  }

  std::string key = "shown_msg_" + ToString(msgNumber);
  if (p->Exists(key))
  {
    // Already shown this msg
    return false;
  } 

  // Set shown flag
  TheObjectUpdater::Instance()->SendUpdateReq(p->GetId(), key, "y");
  p->SetKeyVal(key, "y");
  // TODO set shown flag when msg closed ?

  ShowText(text, storyMode);
  return true;
}

bool FirstTimeMsgThisSession(const std::string& text, MsgNum msgNumber, bool storyMode)
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

  ShowText(text, storyMode);
  return true;
}
}

