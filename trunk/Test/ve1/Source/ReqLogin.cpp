#include "ReqLogin.h"
#include "Ve1OnlineReqManager.h"
#include <iostream>
#include <Xml/XmlParser2.h>
#include <Xml/XmlNodeInterface.h>
#include <SafeUtils.h>
#include <Game.h>
#include "GSStartGame.h"

namespace Amju
{
ReqLogin::ReqLogin(const std::string& url) : Ve1Req(url, "login")
{
}

void ReqLogin::HandleResult()
{
  // Check for session ID or login failure
  TheVe1ReqManager::Instance()->SetLoggedIn(false);
  HttpResult res = GetResult();
  if (res.GetSuccess())
  {
std::cout << "Login result: " << res.GetString() << "\n";

    // Parse XML, create Object and add to ObjectManager
    PXml xml = ParseXml(res.GetString().c_str());

    // Format of XML:
    // <session/>           <- Child(0)

    PXml p = xml.getChildNode(0);
    if (SafeStrCmp(p.getName(), "session"))
    {
#ifdef XML_DEBUG
std::cout << "found session element\n";
#endif
      std::string sessionId = p.getText();

      std::cout << "Got session ID! " << sessionId << "\n"; 

      TheVe1ReqManager::Instance()->SetSessionId(sessionId);
      TheVe1ReqManager::Instance()->SetLoggedIn(true);

      // Go to the main game, we are now logged in OK!
      TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());
    }
    else
    {
std::cout << "Didn't get sesssion ID from server :-(\n";
    }
  }
  else  
  {
std::cout << "Failed to log in! Error: " << res.GetErrorString() << "\n";
    // TODO show error page
  }

}

void SendLoginReq(const std::string& email)
{
  // TODO Sanitize email addr
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(LOGIN_REQ) + "?email=" + email;

  TheVe1ReqManager::Instance()->AddLoginReq(new ReqLogin(url));
}

} // namespace
