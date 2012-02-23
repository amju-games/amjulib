#ifndef LOCAL_PLAYER_H_INCLUDED
#define LOCAL_PLAYER_H_INCLUDED

#include "Player.h"

namespace Amju
{
void SetLocalPlayerId(int localPlayerId);

Player* GetLocalPlayer();

// It's important to know if an object is in the same location as the local player, but the local
//  player may not exist yet! What a nightmare!
// So keep the local player location stored, whether or nor the local player object exists.
int GetLocalPlayerLocation();
void SetLocalPlayerLocation(int lpl);
}

#endif

