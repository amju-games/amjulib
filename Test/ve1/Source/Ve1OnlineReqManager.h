#ifndef VE1_ONLINE_REQ_MANAGER_H
#define VE1_ONLINE_REQ_MANAGER_H

#include "Ve1Req.h"
#include <Singleton.h>
#include <RCPtr.h>

namespace Amju
{
enum Task
{
  LOGIN,

  GET_NEW_OBJECTS,

  SET_POSITION,
  GET_POSITION_UPDATES,

  SET_STATE,  // send a key/val to store in DB
  GET_STATE_UPDATES,  // get all key/val updates since last check

  SEND_MSG,
  GET_NEW_MSGS,
  MARK_MSG_READ,
};

class Ve1ReqManager : public OnlineReqManager
{
public:
  Ve1ReqManager();

  void SetLoggedIn(bool);
  bool IsLoggedIn() const;

  // Make URL for connecting to server
  std::string MakeUrl(Task);

  // Make URL for simple file download request - no authentication
  std::string MakeDownloadUrl(const std::string& filename);

  //void Update() {}

  // Check for valid log in before sending req
  bool AddReq(RCPtr<OnlineReq> req, int maxConcurrentReqs = 1);

  // Login req is special, as you don't need to be logged in to send it :-)
  bool AddLoginReq(RCPtr<OnlineReq> req); 

  void SetSessionId(const std::string& session);

private:
  bool m_isLoggedIn;
  std::string m_session;
};

typedef Singleton<Ve1ReqManager> TheVe1ReqManager;
}

#endif

