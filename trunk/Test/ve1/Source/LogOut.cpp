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
std::cout << "Logging out, so first clearing request queue...\n";
    TheVe1ReqManager::Instance()->Clear();

std::cout << "Now sending log out request (blocking)...\n";

    // Not threaded - so we definitely send request on exit ?
    std::string url = TheVe1ReqManager::Instance()->MakeUrl(LOGOUT);
  std::cout << "URL: " << url << "\n";

    HttpClient hc;
    HttpResult res;
    hc.Get(url, HttpClient::GET, &res);
std::cout << "Logout response: " << res.GetString() << "\n";

    TheVe1ReqManager::Instance()->SetLoggedIn(false);
  }
}
}
