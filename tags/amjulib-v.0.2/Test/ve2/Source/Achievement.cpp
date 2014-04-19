#include <AmjuFirst.h>
#include <set>
#include <utility>
#include <AmjuAssert.h>
#include "Achievement.h"
#include "HeartCount.h"
#include "LocalPlayer.h"
#include "LurkMsg.h"
#include "GameLookup.h"
#include "ObjectUpdater.h"
#include <AmjuFinal.h>

namespace Amju
{
static const std::string NUM_ACHIEVEMENTS = "num_ach";

// Local cache of ach. flags
typedef std::set<std::pair<int, int> > AchSet; // player ID, ach ID
static AchSet achSet;

std::string MakeKey(int a)
{
  return "ach_" + ToString(a);
}

void SetAchievement(int a)
{
  int lpid = GetLocalPlayerId();
  if (achSet.count(std::make_pair(lpid, a)) == 0)
  {
    // Store flag on server, and cache 
    TheObjectUpdater::Instance()->SendUpdateReq(lpid, MakeKey(a), "y");
  
    achSet.insert(std::make_pair(GetLocalPlayerId(), a));
  }
}

bool HasWonAchievement(int a)
{
  if (achSet.count(std::make_pair(GetLocalPlayerId(), a)) > 0)
  {
    return true;
  }
  Player* lp = GetLocalPlayer();
  Assert(lp);
  if (lp->Exists(MakeKey(a)))
  {
    achSet.insert(std::make_pair(GetLocalPlayerId(), a));
    return true;
  }
  return false;
}

void OnWinAchievement(int achievement, const std::string& msg)
{
  Assert(!HasWonAchievement(achievement));

  SetAchievement(achievement);

  // Increment count safely
  ChangePlayerCount(NUM_ACHIEVEMENTS, 1);

  static const Colour ACH_FG(0, 0, 0, 1);
  static const Colour ACH_BG(1, 1, 0, 1);

  LurkMsg lm(GameLookup(std::string("You got an achievement!\n") + msg), ACH_FG, ACH_BG, AMJU_CENTRE);
  TheLurker::Instance()->Queue(lm);
}

int GetNumAchievements()
{
  int r = 0;
  bool b = GetPlayerCount(NUM_ACHIEVEMENTS, &r);
  Assert(b);
  return r;
}

const char* GetAchievementName(AchCode a)
{
  Assert(a < ACH_MAX);

  /*
  ACH_FUEL_CELL_TO_SHIP_1,
  ACH_FUEL_CELL_TO_SHIP_5,
  ACH_FUEL_CELL_TO_SHIP_10,
  ACH_FUEL_CELL_TO_SHIP_20,
  ACH_FUEL_CELL_TO_SHIP_50,
  ACH_FUEL_CELL_TO_SHIP_100,

  ACH_MSGS_SENT_1,
  ACH_MSGS_SENT_5,
  ACH_MSGS_SENT_10,
  ACH_MSGS_SENT_20,
  ACH_MSGS_SENT_50,
  ACH_MSGS_SENT_100,
  */
  
  static const char* ACH_NAME[] = 
  {
    "Brought your first fuel cell to the spaceship",
    "Brought 5 fuel cells to the spaceship",
    "Brought 10 fuel cells to the spaceship",
    "Brought 20 fuel cells to the spaceship",
    "Brought 50 fuel cells to the spaceship",
    "Brought 100 fuel cells to the spaceship",
    "Sent 1 message",
    "Sent 5 messages",
    "Sent 10 messages",
    "Sent 20 messages",
    "Sent 50 messages",
    "Sent 100 messages",
  };
  return ACH_NAME[a]; // TODO localise
}

}

