#include "MsgManager.h"
#include <iostream>
#include "Ve1OnlineReqManager.h"
#include <StringUtils.h>
#include "ReqSendMsg.h"
#include "ReqGetNewMsgs.h"
#include "GSMain.h"
#include <Timer.h>
#include <UrlUtils.h>
#include "Player.h"
#include <Game.h>

namespace Amju
{
static const float MAX_CHECK_PERIOD = 5.0f;

MsgManager::MsgManager()
{
  m_elapsed = 0;
  m_checkPeriod = MAX_CHECK_PERIOD;
}

void MsgManager::QueueMsg(const Msg& msg)
{
  m_map[msg.m_whenSent] = msg;
}

void MsgManager::Update()
{
  // TODO Check periodically

  // Period can be short cut if we might be in a conversation

  float dt = TheTimer::Instance()->GetDt();
  m_elapsed += dt;
  if (m_elapsed > m_checkPeriod)
  {
    m_elapsed = 0;

    // Get Msgs with ID greater than last msg ID recved
    SendGetNewMsgsReq();
    // Queue new msgs  -- in OnSuccess() in ReqGetNewMsgs
  }



  // Check if any new msgs in queue
  if (m_map.empty())
  {
    return;
  }

  GSMain* gsm = TheGSMain::Instance();
  // Can we display the new msg ?
  if (gsm->CanShowMsg())
  {
    // Tell the main game state that we have a new msg 
    Msgs::iterator it = m_map.begin();
    Assert(it != m_map.end());
    Msg& msg = it->second;
    gsm->ShowMsg(msg);
    m_map.erase(it);
  }
}

void MsgManager::SendMsg(int senderId, int recipId, const std::string& msg)
{
  Assert(senderId != -1);
  Assert(recipId != -1);
  // For now, assume senders and recips are Players
  Assert(dynamic_cast<Player*>(TheGame::Instance()->GetGameObject(senderId).GetPtr()));
  Assert(dynamic_cast<Player*>(TheGame::Instance()->GetGameObject(recipId).GetPtr()));

  // Replace spaces in msg 
  // TODO what about punctuation chars.. they are stripped out at server, need special codes like HTML
  std::string newmsg = Replace(msg, " ", "_");

std::cout << "Sending msg: to: " << recipId << " From: " << senderId << " msg: " << msg << "\n";

  static const int MAX_CONCURRENT_MSGS = 1; // ?

  std::string url = TheVe1ReqManager::Instance()->MakeUrl(SEND_MSG);
  url += "&recip=";
  url += ToString(recipId);
  url += "&sender=";
  url += ToString(senderId);
  url += "&msg='";
  url += newmsg;
  url += "'";
  TheVe1ReqManager::Instance()->AddReq(new ReqSendMsg(ToUrlFormat(url)), MAX_CONCURRENT_MSGS);
}
}

