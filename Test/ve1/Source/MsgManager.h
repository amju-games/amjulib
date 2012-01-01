#ifndef MSG_MANAGER_H
#define MSG_MANAGER_H

#include <Singleton.h>
#include <string>
#include <map>
#include "Timestamp.h"

namespace Amju
{
class MsgManager : public NonCopyable
{
public:
  void Update();
  void SendMsg(int senderId, int recipId, const std::string& msg);
  
  struct Msg
  {
    Msg() : m_id(-1), m_senderId(-1), m_whenSent(1)  {}

    Msg(int id, const std::string& text, int senderId, Timestamp whenSent) :
      m_id(id), m_text(text), m_senderId(senderId), m_whenSent(whenSent) {}

    int m_id;
    std::string m_text;
    int m_senderId;
    Timestamp m_whenSent;
    // Type, e.g. post, chat, pigeon post
  };

private:

  friend class Singleton<MsgManager>;
  MsgManager();

  friend class ReqGetNewMsgs;
  void QueueMsg(const Msg& msg);

private:

  typedef std::map<Timestamp, Msg> Msgs;
  Msgs m_map;
  float m_elapsed;
  float m_checkPeriod;
};

typedef Singleton<MsgManager> TheMsgManager;
}

#endif

