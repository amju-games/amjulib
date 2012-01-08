#include "LocalPlayer.h"
#include <Game.h>

namespace Amju
{
static int localPlayerId = -1;

Player* GetLocalPlayer()
{
  GameObject* p = TheGame::Instance()->GetGameObject(localPlayerId);
  if (p)
  {
    Assert(dynamic_cast<Player*>(p));
    ((Player*)p)->SetIsLocalPlayer(true);
  }
  return (Player*)p;
}

void SetLocalPlayerId(int id)
{
  localPlayerId = id;
}

/*
void OnNewObject()
{
  // Show new object menu
  OnBuildButton(); // same as if Build button clicked
}

void OnBuildButton(); // in GSMain

const char* LocalPlayer::TYPENAME = "local-player";
  
LocalPlayer::LocalPlayer()
{
  localplayer = this;
}

void LocalPlayer::SetMenu(GuiMenu* menu)
{
  menu->AddItem(new GuiMenuItem("Add a new object...", OnNewObject));
}
*/
}


