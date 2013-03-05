#ifndef HEART_COUNT_H_INCLUDED
#define HEART_COUNT_H_INCLUDED

#include <string>

namespace Amju
{
// TODO Not sure if we will use heart count for anything.. leaving it in for now.
// Get heart count for local player, if available
bool GetHeartCount(int* result);

// Change heart count - this updates the server
bool ChangeHeartCount(int delta);

bool GetFoodCount(int* result);
bool ChangeFoodCount(int delta);



// Change a numeric attribute for the local player
bool ChangePlayerCount(const std::string& key, int delta);

bool GetPlayerCount(const std::string& key, int* result);

// Add delta to key for any object
bool ChangeObjCount(int objId, const std::string& key, int delta);

}

#endif

