#include "Ve1OnlineReqManager.h"

namespace Amju
{
// TODO URL creator object
std::string MakeDownloadUrl(const std::string& filename)
{
  std::string s = "www.amju.com/ve1/assets/" + filename;
  return s;
}

std::string MakeUrl(Task t)
{
  // TODO CONFIG
  std::string s =  "www.amju.com/ve1/cgi-bin/";
  
  // Add Task-specific CGI script name
  switch (t)
  {
  case MOVE_REQ:
    s += "movereq.pl"; // Calling code adds object ID, new location req
    break;

  case OBJECT_CHECK_REQ:
    s += "getobjects.pl"; // Calling code adds time of last check, (region)
    break;
  }


  // TODO Authentication, player ID, session ID, etc.

  return s;
}

}

