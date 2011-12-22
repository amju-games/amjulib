#include "ReqLogin.h"
#include "Ve1OnlineReqManager.h"
#include <iostream>
#include <Xml/XmlParser2.h>
#include <Xml/XmlNodeInterface.h>
#include <SafeUtils.h>
#include <Game.h>
#include "GSStartGame.h"
#include "GSLoginWaiting.h"
#include "PlayerInfo.h"
#include "GSAvatarMod.h"
#include "GSLogin.h"

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
//std::cout << "Login result: " << res.GetString() << "\n";

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
      std::string playername;
      int objId = -1; // object ID for local player

      // Get player name and object ID for local player
      p = xml.getChildNode(1);
      if (SafeStrCmp(p.getName(), "playername"))
      {
        playername = p.getText();
std::cout << "**Got player name! \"" << playername << "\"\n";
      }
      else
      {
        // Got session ID but no player name, WTF ?
      }

      p = xml.getChildNode(2);
      if (SafeStrCmp(p.getName(), "objid"))
      {
        objId = atoi(p.getText());
std::cout << "**Got local player object ID: " << objId << "\n";
      }
      else
      {
        // Got session ID but we don't know the object ID for the local player. WTF ?
      }

      std::cout << "Got session ID! " << sessionId << "\n"; 

      TheVe1ReqManager::Instance()->SetSessionId(sessionId);
      TheVe1ReqManager::Instance()->SetLoggedIn(true);

      // Check if we are set up
      std::string playerInfoFilename = playername + ".txt";
      // TODO Sanitise the filename

      ThePlayerInfoManager::Instance()->SetCurrentPlayer(playerInfoFilename);
      PlayerInfo* pi = ThePlayerInfoManager::Instance()->GetPI(); 

      pi->PISetInt(PI_KEY("player obj id"), objId);
      pi->PISetString(PI_KEY("playername"), playername);
      // TODO Do we need to set this via ObjectUpdater too, so it gets sent to all clients ?

      Assert(pi);
      if (pi->PIGetBool(PI_KEY("has set up")))
      {
        // Go to the main game, we are now logged in OK!
        TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());
      }
      else
      {
        // Go to set up page
        TheGSAvatarMod::Instance()->SetPrevState(TheGSLogin::Instance());
        TheGame::Instance()->SetCurrentState(TheGSAvatarMod::Instance());
      }
    }
    else
    {
std::cout << "Didn't get sesssion ID from server :-(\n";
      TheGSLoginWaiting::Instance()->SetErrorString("Didn't get session ID from server");
    }
  }
  else  
  {
std::cout << "Failed to log in! Error: " << res.GetErrorString() << "\n";
    // TODO show error page
    TheGSLoginWaiting::Instance()->SetErrorString(res.GetErrorString());
  }

}

void SendLoginReq(const std::string& email)
{
  // TODO Sanitize email addr
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(LOGIN) + "?email=" + email;

  TheVe1ReqManager::Instance()->AddLoginReq(new ReqLogin(url));
}

} // namespace
