#ifndef ACHIEVEMENT_H_INCLUDED
#define ACHIEVEMENT_H_INCLUDED

#include <string>

namespace Amju
{
// Sets achievement flag for local player. Show msg in special colours.
void OnWinAchievement(int achievement, const std::string& msg);

bool HasAchievement(int achievment);

// How many achievements won in total ?
int GetNumAchievements();
}

#endif

