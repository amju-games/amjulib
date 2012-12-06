#ifndef PLAYER_NAMES_H_INCLUDED
#define PLAYER_NAMES_H_INCLUDED

#include <string>

namespace Amju
{
// Store names of all players, even if object is not in mem.
void SetPlayerName(int objId, const std::string& name);

const std::string& GetPlayerName(int objId);
}

#endif

