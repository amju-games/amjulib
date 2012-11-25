#ifndef MSG_MANAGER_H
#define MSG_MANAGER_H

#include <set>
#include <string>
#include <map>
#include <Singleton.h>
#include "Timestamp.h"

namespace Amju
{
// Controls sending and receiving of chat messages
class MsgManager : public NonCopyable
{
public:
  void Update();
  void SendMsg(int senderId, int recipId, const std::string& msg);
  
  struct Msg
  {
    Msg() : m_id(-1), m_senderId(-1), m_recipId(-1), m_whenSent(1)  {}

    Msg(int id, const std::string& text, int senderId, int recipId, Timestamp whenSent) :
      m_id(id), m_text(text), m_senderId(senderId), m_recipId(recipId), m_whenSent(whenSent) {}

    int m_id;
    std::string m_text;
    int m_senderId;
    int m_recipId;
    Timestamp m_whenSent;
    // Type, e.g. post, chat, pigeon post
  };

private:

  friend class Singleton<MsgManager>;
  MsgManager();

  friend class ReqGetNewMsgs;
  void QueueMsg(const Msg& msg);

  void MarkRead(const Msg& msg); // flag as read on server

private:

  typedef std::multimap<Timestamp, Msg> Msgs;
  Msgs m_map;
  float m_elapsed;
  float m_checkPeriod;

  // IDs of msgs received from server
  std::set<int> m_msgsRecv;
};

std::string EncodeMsg(const std::string& plainMsg);
std::string DecodeMsg(const std::string& encodedMsg);

typedef Singleton<MsgManager> TheMsgManager;
}

#endif

