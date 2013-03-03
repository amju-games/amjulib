#ifndef LOCAL_PLAYER_H_INCLUDED
#define LOCAL_PLAYER_H_INCLUDED

#include "Player.h"

namespace Amju
{
// Use this to create a Local Player object when we are doing cog tests only.
void CreateDummyLocalPlayer();

void SetLocalPlayerId(int localPlayerId);
int GetLocalPlayerId();

Player* GetLocalPlayer();

// It's important to know if an object is in the same location as the local player, but the local
//  player may not exist yet! What a nightmare!
// So keep the local player location stored, whether or nor the local player object exists.
int GetLocalPlayerLocation();
void SetLocalPlayerLocation(int lpl);

// Call this to reset local player to known good start position
void ResetLocalPlayer();

extern const char* TOTAL_PLAY_TIME_KEY;
}

#endif

