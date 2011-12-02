#include "LocalPlayer.h"

namespace Amju
{
const char* LocalPlayer::TYPENAME = "local-player";

LocalPlayer* GetLocalPlayer()
{
  // TODO
  static LocalPlayer* lp = new LocalPlayer;
  return lp;
}

}


