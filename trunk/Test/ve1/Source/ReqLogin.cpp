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
#include "GSNetError.h"
#include "GSTitle.h"
#include "LocalPlayer.h"
#include "GSChoosePlayer.h"

namespace Amju
{
ReqLogin::ReqLogin(const std::string& url, const std::string& email) : Ve1Req(url, "login"), m_email(email)
{
}

void ReqLogin::OnFailure()
{
  ShowError("Couldn't log in. " + m_errorStr, TheGSTitle::Instance());
}

void ReqLogin::OnSuccess()
{
  // Check for session ID or login failure
  TheVe1ReqManager::Instance()->SetLoggedIn(false);

  // Format of XML:
  // <session/>           <- Child(1)

  PXml p = m_xml.getChildNode(1);
  if (SafeStrCmp(p.getName(), "session"))
  {
#ifdef XML_DEBUG
std::cout << "found session element\n";
#endif

    std::string sessionId = p.getText();
    std::string playername;
    int objId = -1; // object ID for local player

    // Get player name and object ID for local player
    p = m_xml.getChildNode(2);
    if (SafeStrCmp(p.getName(), "playername"))
    {
      playername = p.getText();
std::cout << "**Got player name! \"" << playername << "\"\n";
    }
    else
    {
      // Got session ID but no player name, WTF ?
std::cout << "Got session ID but no player name, WTF??\n";
    }

    p = m_xml.getChildNode(3);
    if (SafeStrCmp(p.getName(), "objid"))
    {
      objId = atoi(p.getText());
std::cout << "**Got local player object ID: " << objId << "\n";
    }
    else
    {
        // Got session ID but we don't know the object ID for the local player. WTF ?
std::cout << "Got session ID but we don't know the object ID for the local player. WTF?\n";
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
    pi->PISetString(PI_KEY("email"), m_email);
    // TODO Do we need to set this via ObjectUpdater too, so it gets sent to all clients ?

    // Set ID of this player object as the local player ID
    SetLocalPlayerId(objId);

    Assert(pi);
    // TODO No good, this should be a flag on server
    /*
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
    */

    // Always go to avatar page..????
    // Problems here - we don't yet have the player's avatar state from the server, so 
    //  display the default (white cat). 
    //TheGSAvatarMod::Instance()->SetPrevState(TheGSChoosePlayer::Instance());
    //TheGame::Instance()->SetCurrentState(TheGSAvatarMod::Instance());

    TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());

  }
  else
  {
std::cout << "Didn't get sesssion ID from server :-(\n";
      TheGSLoginWaiting::Instance()->SetErrorString("Didn't get session ID from server");
  }
}

void SendLoginReq(const std::string& email)
{
  // TODO Sanitize email addr
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(LOGIN) + "?email=" + email;

  TheVe1ReqManager::Instance()->AddLoginReq(new ReqLogin(url, email));
}

} // namespace
