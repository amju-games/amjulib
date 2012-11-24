#include <set>
#include <utility>
#include "Achievement.h"
#include "HeartCount.h"
#include "LocalPlayer.h"
#include "LurkMsg.h"
#include "GameLookup.h"
#include "ObjectUpdater.h"

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
}

