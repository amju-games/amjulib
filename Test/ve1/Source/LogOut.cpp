#include <HttpClient.h>
#include "LogOut.h"
#include "Ve1OnlineReqManager.h"

namespace Amju
{
void SendLogOut()
{
  // Not threaded - so we definitely send request on exit ?
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(LOGOUT);
  HttpClient hc;
  HttpResult res;
  hc.Get(url, HttpClient::GET, &res);
  std::cout << "Logout: " << res.GetString() << "\n";
}
}
