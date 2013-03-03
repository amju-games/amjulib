#ifndef GAME_MODE_H_INCLUDED
#define GAME_MODE_H_INCLUDED

namespace Amju
{
enum GameMode
{
  AMJU_MODE_NONE,
  AMJU_MODE_NO_GAME, // non-game mode, to establish baseline
  AMJU_MODE_SINGLE,  
  AMJU_MODE_MULTI,
  AMJU_MODE_EDIT     // edit locations/game objects 
};

void SetGameMode(GameMode gm);

GameMode GetGameMode();

bool IsEditMode();

bool DoCogTests();
void SetDoCogTests(bool);
}

#endif

