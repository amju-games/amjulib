#include <iostream>
#include <HttpClient.h>
#include "LogOut.h"
#include "Ve1OnlineReqManager.h"

namespace Amju
{
void SendLogOut()
{
  if (TheVe1ReqManager::Instance()->IsLoggedIn())
  {
std::cout << "Logging out...\n";

    // Not threaded - so we definitely send request on exit ?
    std::string url = TheVe1ReqManager::Instance()->MakeUrl(LOGOUT);
  std::cout << "URL: " << url << "\n";

    HttpClient hc;
    HttpResult res;
    hc.Get(url, HttpClient::GET, &res);
std::cout << "Logout response: " << res.GetString() << "\n";
  }
}
}
