#include "Ve1OnlineReqManager.h"
#include <iostream>

namespace Amju
{
Ve1ReqManager::Ve1ReqManager()
{
  m_isLoggedIn = false;
}

void Ve1ReqManager::SetLoggedIn(bool isLoggedIn)
{
  m_isLoggedIn = isLoggedIn;
}

bool Ve1ReqManager::IsLoggedIn() const
{
  return m_isLoggedIn;
}

std::string Ve1ReqManager::MakeDownloadUrl(const std::string& filename)
{
  std::string s = "www.amju.com/ve1/assets/" + filename;
  return s;
}

std::string Ve1ReqManager::MakeUrl(Task t)
{
  // TODO CONFIG
  std::string s =  "www.amju.com/ve1/cgi-bin/";
  
  // Add Task-specific CGI script name
  switch (t)
  {
  case LOGIN:
    s += "login.pl";
    // ** NB return ** -- we don't have a session yet!
    return s;

  case SET_POSITION:
    s += "movereq.pl"; // Calling code adds object ID, new location req
    break;

  case GET_NEW_OBJECTS:
    s += "getobjects.pl"; // Calling code adds time of last check, (region)
    break;

  case GET_POSITION_UPDATES:
    s += "getpos.pl"; // Calling code adds time of last check, (region)
    break;

  case GET_STATE_UPDATES:
    s += "getstates.pl"; // calling code add time of last check
    break;

  case SET_STATE:
    s += "updatereq.pl"; // calling code adds object ID, key, val
    break;

  case SEND_MSG:
    s += "sendmsg.pl"; // calling code adds recip ID, msg
    break;

  case GET_NEW_MSGS:
    s += "getmsgs.pl";
    break;

  case MARK_MSG_READ:
    s += "markread.pl";
    break;
  }

  // TODO Authentication, player ID, session ID, etc.
  s += "?session_id=" + m_session;

  return s;
}

void Ve1ReqManager::SetSessionId(const std::string& session)
{
  m_session = session;
}

bool Ve1ReqManager::AddLoginReq(RCPtr<OnlineReq> req)
{
  return OnlineReqManager::AddReq(req, 1);
}

bool Ve1ReqManager::AddReq(RCPtr<OnlineReq> req, int maxConcurrentReqs)
{
  // Fail here if we are not logged in
  if (!IsLoggedIn())
  {
std::cout << "* Can't make request, not logged in *\n";
    return false;
  }

  return OnlineReqManager::AddReq(req, maxConcurrentReqs);

//  // No-threads version
//  req->Work();
//  return true;
}

}

