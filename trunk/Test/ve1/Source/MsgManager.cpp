#include "MsgManager.h"
#include <iostream>
#include <StringUtils.h>
#include <Timer.h>
#include <UrlUtils.h>
#include <Game.h>
#include <StringUtils.h>
#include "ReqSendMsg.h"
#include "ReqGetNewMsgs.h"
#include "GSMain.h"
#include "Player.h"
#include "ReqMsgRead.h"
#include "ChatConsole.h"
#include "BroadcastConsole.h"
#include "Ve1OnlineReqManager.h"
#include "TextToSpeech.h"
#include "ROConfig.h"

#define SEND_DEBUG

namespace Amju
{
MsgManager::MsgManager()
{
  m_elapsed = 0;

  static const float MAX_CHECK_PERIOD = ROConfig()->GetFloat("chat-poll-period", 2.0f);
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

  static ChatConsole* cc = TheChatConsole::Instance();
  static BroadcastConsole* bc = TheBroadcastConsole::Instance();

  // First look for broadcast msgs
  for (Msgs::iterator it = m_map.begin(); it != m_map.end(); )
  {
    Msg& msg = it->second;
    if (msg.m_recipId < 0)
    {
      // Broadcast
      std::string name;
      std::string str;
      if (GetNameForPlayer(msg.m_senderId, &name))
      {
        str = name + ": ";
      }
      str += msg.m_text; 
      bc->OnMsgRecv(str);

      m_map.erase(it++); // which doesn't invalidate iterator, right ??
    }
    else
    {
      ++it;
    }
  }

  // Show remaining msgs in chat console if allowed
  // Can we display the new msg ?
  if (cc->CanShowMsg())
  {
    // Tell the main game state that we have a new msg 
    Msgs::iterator it = m_map.begin();
    if (it != m_map.end())
    {
      Msg& msg = it->second;
      //gsm->ShowMsg(msg);
      cc->ActivateChatRecv(true, &msg);
      cc->ActivateChatSend(true, msg.m_senderId);

      // Text to speech -- NB is here the best place ?
      TextToSpeech(msg.m_text);

      // Mark message as read -- TODO only after player clicks OK in gui..?
      if (m_map.size() == 1)
      {
        // Last (i.e. most recent) msg - mark as read: this should 
        //  mark all earlier msgs as read also.
        MarkRead(msg);
      }

      m_map.erase(it);
    }
  }
}

void MsgManager::MarkRead(const Msg& msg)
{
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(MARK_MSG_READ);
  url += "&id=";
  url += ToString(msg.m_id);
  TheVe1ReqManager::Instance()->AddReq(new ReqMsgRead(url), 1); // TODO only need 1 ??
}

void MsgManager::SendMsg(int senderId, int recipId, const std::string& msg)
{
  Assert(senderId != -1);
  Assert(recipId != -1);
  // For now, assume senders and recips are Players
  Assert(dynamic_cast<Player*>(TheGame::Instance()->GetGameObject(senderId).GetPtr()));
  if (recipId >= 0)
  {
    Assert(dynamic_cast<Player*>(TheGame::Instance()->GetGameObject(recipId).GetPtr()));
  }
  // Replace spaces in msg 
  // TODO what about punctuation chars.. they are stripped out at server, need special codes like HTML
  std::string newmsg = EncodeMsg(msg); ////Replace(msg, " ", "_");

  // Strip out characters which are not allowed - use boost reg exp
  // Replace punctuation chars with ascii code.

#ifdef SEND_DEBUG
std::cout << "Sending msg: to: " << recipId << " From: " << senderId << " msg: " << msg << "\n";
#endif

  static const int MAX_CONCURRENT_MSGS = 1; // ?

  std::string url = TheVe1ReqManager::Instance()->MakeUrl(SEND_MSG);
  url += "&recip=";
  url += ToString(recipId);
  url += "&sender=";
  url += ToString(senderId);
  url += "&msg='";
  url += newmsg;
  url += "'";
  TheVe1ReqManager::Instance()->AddReq(new ReqSendMsg(url), MAX_CONCURRENT_MSGS);
}

std::string EncodeMsg(const std::string& plainMsg)
{
  std::string result;

  int s = plainMsg.size();
  for (int i = 0; i < s; i++)
  {
    char c = plainMsg[i];
    result += ToHexString(c);
  }

  return result;
}

std::string DecodeMsg(const std::string& encodedMsg)
{
  std::string result;

  int s = encodedMsg.size();
  for (int i = 0; i < s; i += 2)
  {
    std::string hex = encodedMsg.substr(i, 2);
    int n = UIntFromHexString(hex);
    result += std::string(1, (char)n);
  }

  return result;
}

}

