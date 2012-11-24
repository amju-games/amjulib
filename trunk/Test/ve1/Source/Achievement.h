#ifndef ACHIEVEMENT_H_INCLUDED
#define ACHIEVEMENT_H_INCLUDED

#include <string>

namespace Amju
{
// Achievement codes
enum AchCode
{
  ACH_FUEL_CELL_TO_SHIP_1,
  ACH_FUEL_CELL_TO_SHIP_5,
  ACH_FUEL_CELL_TO_SHIP_10,

  ACH_MSGS_SENT_1,
  ACH_MSGS_SENT_5,
  ACH_MSGS_SENT_10,
  ACH_MSGS_SENT_20,
  ACH_MSGS_SENT_50,
  ACH_MSGS_SENT_100,

};

// Sets achievement flag for local player. Show msg in special colours.
void OnWinAchievement(int achievement, const std::string& msg);

bool HasWonAchievement(int achievment);

// How many achievements won in total ?
int GetNumAchievements();
}

#endif

