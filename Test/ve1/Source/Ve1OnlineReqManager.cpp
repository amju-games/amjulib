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
  case LOGIN_REQ:
    s += "login.pl";
    // ** NB return ** -- we don't have a session yet!
    return s;

  case MOVE_REQ:
    s += "movereq.pl"; // Calling code adds object ID, new location req
    break;

  case OBJECT_CHECK_REQ:
    s += "getobjects.pl"; // Calling code adds time of last check, (region)
    break;

  case POS_UPDATE_REQ:
    s += "getpos.pl"; // Calling code adds time of last check, (region)
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

