#include "GameMode.h"

namespace Amju
{
static GameMode s_gm = AMJU_MODE_NONE;

void SetGameMode(GameMode gm)
{
  s_gm = gm;
}

GameMode GetGameMode()
{
  return s_gm;
}

bool IsEditMode()
{
  return s_gm == AMJU_MODE_EDIT;
}

static bool cogTests = false;

bool DoCogTests()
{
  return cogTests;
}

void SetDoCogTests(bool c)
{
  cogTests = c;
}

}

