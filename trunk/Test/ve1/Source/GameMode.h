#ifndef GAME_MODE_H_INCLUDED
#define GAME_MODE_H_INCLUDED

namespace Amju
{
enum GameMode
{
  AMJU_MODE_LOCAL,
  AMJU_MODE_NO_GAME, // non-game mode, to establish baseline
  AMJU_MODE_SINGLE,  
  AMJU_MODE_MULTI
};

void SetGameMode(GameMode gm);

GameMode GetGameMode();

bool IsOnline();
bool IsOffline();
}

#endif

