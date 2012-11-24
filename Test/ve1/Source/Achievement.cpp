#include "Achievement.h"
#include "HeartCount.h"
#include "LocalPlayer.h"
#include "LurkMsg.h"
#include "GameLookup.h"

namespace Amju
{
static const std::string NUM_ACHIEVEMENTS = "num_ach";

void SetAchievement(int a)
{
}

void OnWinAchievement(int achievement, const std::string& msg)
{
  Assert(!HasAchievement(achievement));

  SetAchievement(achievement);

  // Increment count safely
  ChangePlayerCount(NUM_ACHIEVEMENTS, 1);

  static const Colour ACH_FG(0, 0, 0, 1);
  static const Colour ACH_BG(1, 1, 0, 1);

  LurkMsg lm(GameLookup(msg), ACH_FG, ACH_BG, AMJU_CENTRE);
  TheLurker::Instance()->Queue(lm);
}

bool HasAchievement(int achievement)
{
  return false;
}

int GetNumAchievements()
{
  int r = 0;
  bool b = GetPlayerCount(NUM_ACHIEVEMENTS, &r);
  Assert(b);
  return r;
}
}

