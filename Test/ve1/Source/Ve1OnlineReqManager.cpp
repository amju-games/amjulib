#include "Ve1OnlineReqManager.h"
#include <iostream>
#include "SpecialConfig.h"
#include "GameMode.h"
#include "LogOut.h"

//#define REQ_DEBUG

namespace Amju
{
std::string UrlRoot()
{
  static bool first = true;
  static std::string res;
  if (first)  // just do look ups once
  {
    first = false;
    std::string server = GetServer();
    std::string env = GetEnv();
    res = "http://" + server + "/" + env + "/";
  }
  return res;
}

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
  std::string s = UrlRoot() + "assets/" + filename;
  return s;
}

std::string Ve1ReqManager::MakeUrl(Task t)
{
  // TODO CONFIG
  std::string s =  UrlRoot() + "cgi-bin/";
  
  // Add Task-specific CGI script name
  switch (t)
  {
  case LOGIN:
    {
      // Get SendLogOut() to be called on exit - only register the function once
      static int registerLogout = true;
      if (registerLogout)
      {
        atexit(SendLogOut);
        registerLogout = false;
      }
    }
    s += "login.pl";
    // ** NB return ** -- we don't have a session yet!
    return s;

  case LOGOUT:
    s += "logout.pl";
    break;

  case SET_POSITION:
    s += "movereq.pl"; // Calling code adds object ID, new location req
    break;
  
  case CHANGE_LOCATION:
    s += "changelocation.pl";
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

  case FILE_UPLOAD:
    s += "myfileup.pl";
    break;

  case GET_NEW_LOCATION_ID:
    s += "newlocation_id.pl";
    break;

  case CREATE_LOCATION:
    s += "newlocation_row.pl";
    break;

  case GET_ALL_LOCATIONS:
    s += "getlocations.pl";
    break;

  case GET_NEW_OBJECT_ID:
    s += "newobject_id.pl";
    break;

  case CREATE_OBJECT:
    s += "newobject_row.pl";
    break;

  case DELETE_OBJECT:
    s += "deleteobject.pl";
    break;

  case FILE_UPDATE_CHECK:
    s += "getupdatedfiles.pl";
    break;

  case SEND_TEST_RESULT:
    s += "storeresult.pl";
    break;

  case FIGHT:
    s += "fightreq.pl";
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

const std::string& Ve1ReqManager::GetSessionId() const
{
  return m_session;
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
#ifdef REQ_DEBUG
std::cout << "* Can't make request, not logged in *\n";
#endif

    return false;
  }

  //if (IsOffline())
  //{
  //  return true;
  //}

  return OnlineReqManager::AddReq(req, maxConcurrentReqs);

//  // No-threads version
//  req->Work();
//  return true;
}

}

