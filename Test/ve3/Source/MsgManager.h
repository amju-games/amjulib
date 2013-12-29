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

  typedef std::multimap<Timestamp, Msg> Msgs;

public:
  void Update(); // check queue
  void CheckForNewMsgs();  // send req to server

  // Return messages for which pred() is true
  template <class Pred> Msgs GetMsgs(Pred pred) const;

  // Special sender/recipient codes for SendMsg()
  static const int BROADCAST_RECIP = -2; // use as recip ID
  static const int SYSTEM_SENDER = -3; // use as sender ID

  void SendMsg(int senderId, int recipId, const std::string& msg);
  
  void QueueMsg(const Msg& msg);

  bool HasNewMsgs() const;
  void ResetNewMsgFlag();

private:

  friend class Singleton<MsgManager>;
  MsgManager();

  void MarkRead(const Msg& msg); // flag as read on server

private:

  typedef std::multimap<Timestamp, Msg> Msgs;
  Msgs m_map;

  // IDs of msgs received from server
  std::set<int> m_msgsRecv;

  bool m_newMsgs;
};

template <class Pred> MsgManager::Msgs MsgManager::GetMsgs(Pred pred) const
{
  Msgs msgs;
  for (Msgs::const_iterator it = m_map.begin(); it != m_map.end(); it++)
  {
    const Msg& msg = it->second;
    if (pred(msg)) 
    {
      msgs.insert(*it);
    }
  }

  return msgs;
}

std::string EncodeMsg(const std::string& plainMsg);
std::string DecodeMsg(const std::string& encodedMsg);

typedef Singleton<MsgManager> TheMsgManager;
}

#endif

