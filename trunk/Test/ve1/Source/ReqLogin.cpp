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
#include "GSFileUpdateCheck.h"
#include "GSSessionInfo.h"
#include "Version.h"
#include "GameMode.h"
#include "GSCogTestMenu.h"

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
      objId = ToInt(p.getText());
std::cout << "**Got local player object ID: " << objId << "\n";
    }
    else
    {
        // Got session ID but we don't know the object ID for the local player. WTF ?
std::cout << "Got session ID but we don't know the object ID for the local player. WTF?\n";
    }

    p = m_xml.getChildNode(4);
    if (SafeStrCmp(p.getName(), "loc"))
    {
      int loc = ToInt(p.getText());
      TheGSStartGame::Instance()->SetStartLoc(loc); 
std::cout << "Got start location: " << loc << "\n";
    }
    else
    {
std::cout << "No start location.\n";
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

    // Handle research info: session, mode, etc.
    GameMode gm = AMJU_MODE_MULTI;
    bool doCogTests = false;

    PXml research = m_xml.getChildNode(5);
    if (SafeStrCmp(research.getName(), "research"))
    {
      p = research.getChildNode(0);
      if (SafeStrCmp(p.getName(), "is_research_session")) 
      {
        int isRS = ToInt(p.getText());
        if (isRS)
        {
          doCogTests = true;
std::cout << "This is a research session so we should do the cog tests before going into the game.\n"; 
        }
        else
        {
std::cout << "This is NOT a research session, woohoo!\n";
        }
      }
      else
      {
std::cout << "Found research element but unexpected format (no is_research_session).\n";
      }

      p = research.getChildNode(2);
      if (SafeStrCmp(p.getName(), "mode"))
      {
        int mode = ToInt(p.getText());
        switch (mode)
        {
        case 0: 
std::cout << "NO GAME MODE!!\n";
          gm = AMJU_MODE_NO_GAME; 
          break;
        
        case 1:
std::cout << "SINGLE MODE!!\n";
          gm = AMJU_MODE_SINGLE;
          break;

        case 2:
std::cout << "MULTI MODE!!\n";
          gm = AMJU_MODE_MULTI;
          break;

        default:
std::cout << "Unexpected mode value.\n";
    
        }
      }
      else
      {
std::cout << "Found research element but unexpected format (no mode).\n";
      }
    }
    else
    {
std::cout << "No research element in login.pl response?!?\n";
      Assert(0);
    }

    SetGameMode(gm); // TODO handle edit mode - send extra flag to login.pl ??

    if (doCogTests)
    {
      TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
    }
    else
    {
      TheGame::Instance()->SetCurrentState(TheGSFileUpdateCheck::Instance());
    }

    // No point ?
    // TheGame::Instance()->SetCurrentState(TheGSSessionInfo::Instance());
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
  // Send version: server can check we are up-to-date
  url += "&clientver=v." + ToString(VersionMajor) + "." + ToString(VersionMinor);

  TheVe1ReqManager::Instance()->AddLoginReq(new ReqLogin(url, email));
}

} // namespace
