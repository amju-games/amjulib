#ifndef HEART_COUNT_H_INCLUDED
#define HEART_COUNT_H_INCLUDED

namespace Amju
{
// Get heart count for local player, if available
bool GetHeartCount(int* result);

// Change heart count - this updates the server
bool ChangeHeartCount(int delta);
}

#endif

