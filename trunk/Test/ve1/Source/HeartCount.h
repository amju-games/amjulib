#ifndef HEART_COUNT_H_INCLUDED
#define HEART_COUNT_H_INCLUDED

#include <string>

namespace Amju
{
// Get heart count for local player, if available
bool GetHeartCount(int* result);

// Change heart count - this updates the server
bool ChangeHeartCount(int delta);

// Change a numeric attribute for the local player
bool ChangePlayerCount(const std::string& key, int delta);
}

#endif

