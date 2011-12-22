#include "LocalPlayer.h"

namespace Amju
{
void OnBuildButton(); // in GSMain

const char* LocalPlayer::TYPENAME = "local-player";
  
static LocalPlayer* localplayer = 0;

LocalPlayer* GetLocalPlayer()
{
  return localplayer;
}

void OnNewObject()
{
  // Show new object menu
  OnBuildButton(); // same as if Build button clicked
}

LocalPlayer::LocalPlayer()
{
  localplayer = this;
}

void LocalPlayer::SetMenu(GuiMenu* menu)
{
  menu->AddItem(new GuiMenuItem("Add a new object...", OnNewObject));
}
}


