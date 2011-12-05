#include "LocalPlayer.h"

namespace Amju
{
const char* LocalPlayer::TYPENAME = "local-player";
  
static LocalPlayer* localplayer = 0;

LocalPlayer* GetLocalPlayer()
{
  return localplayer;
}

LocalPlayer::LocalPlayer()
{
  localplayer = this;
}

}


