#include "GameMode.h"

namespace Amju
{
static GameMode s_gm = AMJU_MODE_LOCAL;

void SetGameMode(GameMode gm)
{
  s_gm = gm;
}

GameMode GetGameMode()
{
  return s_gm;
}


bool IsOnline()
{
  return !IsOffline();
}

bool IsOffline()
{
  return s_gm == AMJU_MODE_LOCAL;
}
}

