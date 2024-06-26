#include <AmjuFirst.h>
#include "Ve1OnlineReqManager.h"
#include <iostream>
#include "SpecialConfig.h"
#include "GameMode.h"
#include "LogOut.h"
#include <AmjuFinal.h>

//#define REQ_DEBUG

namespace Amju
{
std::string AssetUrlRoot()
{
  static bool first = true;
  static std::string res;
  if (first)  // just do look ups once
  {
    first = false;
    std::string server = GetServer();
    // Move from servage to lunarpages: assets in root/ve2/assets/
    std::string env = GetEnv();
    res = "http://" + server + "/" + env + "/assets/";
  }
  return res;
}

std::string ScriptUrlRoot()
{
  static bool first = true;
  static std::string res;
  if (first)  // just do look ups once
  {
    first = false;
    std::string server = GetServer();
    // Move from servage to lunarpages: all scripts must be under cgi-bin
    std::string env = GetEnv();
    res = "http://" + server + "/cgi-bin/" + env + "/";
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
  std::string s = AssetUrlRoot() + filename;
  return s;
}

std::string Ve1ReqManager::MakeUrl(Task t)
{
  if (t == GET_FAST_STATE_UPDATES)
  {
    std::string s = AssetUrlRoot() + "../updates/updates.txt";
    return s;
  }

  std::string s =  ScriptUrlRoot();
  
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

  case GOT_COLLECT:
    s += "gotcollect.pl";
    break;

  case CHANGE_VALUE:
    s += "changevalue.pl";
    break;

  case UPLOAD_MUGSHOT:
    s += "img-upload.pl";
    break;
  }

  // TODO Authentication, player ID, session ID, etc.
  s += "?session_id=" + m_session;
  if (m_session.empty())
  {
    s += "1";
  }

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
  return SerialReqManager::AddReq(req, 1, false);
}

#define THREADS_VERSION

bool Ve1ReqManager::AddReq(RCPtr<OnlineReq> req, int maxConcurrentReqs, bool discardNewNotOld)
{
  // Fail here if we are not logged in
  if (!IsLoggedIn())
  {
#ifdef _DEBUG
std::cout << "* Can't make request, not logged in *\n";
#endif

    return false;
  }

  //if (IsOffline())
  //{
  //  return true;
  //}

#ifdef THREADS_VERSION
  return SerialReqManager::AddReq(req, maxConcurrentReqs, discardNewNotOld);
#endif

 // No-threads versions
#ifdef NO_THREAD_KEEPALIVE
  // This one reuses the same HttpClient
  static HttpClient keepAlive;
  req->DoRequest(keepAlive);     
  req->HandleResult();
#endif

#ifdef NO_THREAD_NO_KEEPALIVE
  req->Work();
#endif

  return true;
}

}

