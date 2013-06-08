#include <AmjuFirst.h>
#include "ReqLogin.h"
#include <TimePeriod.h>
#include "Ve1OnlineReqManager.h"
#include <iostream>
#include <Xml/XmlParser2.h>
#include <Xml/XmlNodeInterface.h>
#include <SafeUtils.h>
#include <SoundManager.h>
#include <Game.h>
#include "GSStartGame.h"
#include "GSLoginWaiting.h"
#include "PlayerInfo.h"
#include "GSLogin.h"
#include "GSNetError.h"
#include "GSTitle.h"
#include "LocalPlayer.h"
#include "GSChoosePlayer.h"
#include "GSFileUpdateCheck.h"
#include "Version.h"
#include "GameMode.h"
#include "GSCogTestMenu.h"
#include "GSOptions.h"
#include "ResearchCalendar.h"
#include "GSThanks.h"
#include "GSCalendar.h"
#include "GSToday.h"
#include "GSMain.h"
#include "CogTestResults.h"
#include "FoodCount.h"
#include "LogOut.h"
#include <AmjuFinal.h>

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
      // TODO Use ResetLocalPlayer() to set start pos/loc ?????
      TheGSStartGame::Instance()->SetStartLoc(loc); 
std::cout << "Got start location: " << loc << "\n";
    }
    else
    {
      // Reset here to well known start location.
      ResetLocalPlayer();
std::cout << "No start location.\n";
    }

    std::cout << "Got session ID! " << sessionId << "\n"; 

    TheVe1ReqManager::Instance()->SetSessionId(sessionId);
    TheVe1ReqManager::Instance()->SetLoggedIn(true);

    // Canceled?
    static Game* game = TheGame::Instance();
    if (game->GetState() != TheGSLoginWaiting::Instance())
    {
      SendLogOut();
      return;
    }

    // Check if we are set up
    std::string playerInfoFilename = playername + ".txt";
    // TODO Sanitise the filename

    ThePlayerInfoManager::Instance()->SetCurrentPlayer(playerInfoFilename);
    ThePlayerInfoManager::Instance()->Save();

    PlayerInfo* pi = ThePlayerInfoManager::Instance()->GetPI(); 

    pi->PISetInt(PI_KEY("player obj id"), objId);
    pi->PISetString(PI_KEY("playername"), playername);
    pi->PISetString(PI_KEY("email"), m_email);
    pi->Save();

    // Set options for this player
    TheGSOptions::Instance()->LoadSettingsFromPI(pi);

    // TODO Do we need to set this via ObjectUpdater too, so it gets sent to all clients ?

    // Set ID of this player object as the local player ID
    SetLocalPlayerId(objId);

    Assert(pi);

    // Play happy logged in sound
    TheSoundManager::Instance()->PlayWav("Sound/button112.wav");

    if (GetGameMode() == AMJU_MODE_EDIT)
    {
      TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());
    }
    else
    {
      // Handle research info: session, mode, etc.
      ChooseMode();
    }
  }
  else
  {
std::cout << "Didn't get sesssion ID from server :-(\n";
    TheGSLoginWaiting::Instance()->SetErrorString("Didn't get session ID from server");
  }
}

void ReqLogin::ChooseMode()
{
  TheResearchCalendar::Instance()->Clear();

  GameMode gm = AMJU_MODE_NO_GAME;
  bool doCogTests = false;

  PXml research = m_xml.getChildNode(5);
  if (SafeStrCmp(research.getName(), "research"))
  {
    // Schedule - for calendar
    PXml p = research.getChildNode(4);
    if (SafeStrCmp(p.getName(), "dates"))
    {
      Time today(Time::Now());
      today.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));

      // Bunch of dates - add to TheResearchCal
      int numDates = p.nChildNode();
      for (int i = 0; i < numDates; i++)
      {
        PXml date = p.getChildNode(i);
        if (SafeStrCmp(date.getName(), "date"))
        {
          if (SafeStrCmp(date.getChildNode(0).getName(), "timestamp") &&
              SafeStrCmp(date.getChildNode(1).getName(), "cogtest") &&
              SafeStrCmp(date.getChildNode(2).getName(), "play"))
          {
            std::string dateStr = date.getChildNode(0).getText();
            bool cogtest = SafeStrCmp(date.getChildNode(1).getText(), "1"); 

            // 'play' field is an enum: 0 for no game (tests only); 
            //  1 for single-player mode; 2 for multi-player mode.
            bool single = SafeStrCmp(date.getChildNode(2).getText(), "1"); 
            bool multi = SafeStrCmp(date.getChildNode(2).getText(), "2"); 
            bool play = single || multi;

            Time t(dateStr);
            t.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));
            if (t == today)
            {
              // Cog tests already done today ?
              Results results = TheCogTestResults::Instance()->GetResultsForDate(Time::Now());
              // TODO Check if only partially done
              if (results.empty())
              {
                doCogTests = cogtest;
              }
              else
              {
                doCogTests = false;
                std::cout << "Test results for today are here already.\n";
              }

              if (multi)
              { 
                gm = AMJU_MODE_MULTI;
              }
              else if (single)
              {
                gm = AMJU_MODE_SINGLE;
              }

              if (doCogTests && !play)
              {
                // Create dummy target for heart count, etc
                CreateDummyLocalPlayer();
              }
            }
            TheResearchCalendar::Instance()->AddResearchDate(ResearchDate(Time(dateStr), cogtest, play));   
          }
          else
          {
            std::cout << "Login: unexpected format for session dates.\n";
            Assert(0);
          }
        }
      }
    }
    else
    {
      std::cout << "Login: found research element but no schedule.\n";
    }
  }
  else
  {
    std::cout << "No research element in login.pl response?!?\n";
    Assert(0);
  }

  // Now we can look up today on the calendar to get game mode flags.

  //ResetLocalPlayerFoodCount();

  SetGameMode(gm); // TODO handle edit mode - send extra flag to login.pl ??

  SetDoCogTests(doCogTests); // mode, in GameMode

  TheGSCalendar::Instance()->SetPrevState(TheGSToday::Instance());
  TheGSThanks::Instance()->SetPrevState(TheGSToday::Instance());
  TheGame::Instance()->SetCurrentState(TheGSThanks::Instance());
  TheGSMain::Instance()->ResetHud();
}

void SendLoginReq(const std::string& email)
{
  // Clear any queued requests
  TheVe1ReqManager::Instance()->Clear();

  // TODO Sanitize email addr
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(LOGIN) + "?email=" + email;
  // Send version: server can check we are up-to-date
  url += "&clientver=v." + ToString(VersionMajor) + "." + ToString(VersionMinor);

  TheVe1ReqManager::Instance()->AddLoginReq(new ReqLogin(url, email));
}

} // namespace
