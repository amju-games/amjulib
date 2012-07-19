#include "LocalPlayer.h"
//#include <Game.h>
#include "ObjectManager.h"

namespace Amju
{
static int localPlayerId = -1;
static int localPlayerLocation = -1;

int GetLocalPlayerLocation()
{
  //Assert(localPlayerLocation > 0);
  return localPlayerLocation; 
}

void SetLocalPlayerLocation(int lpl)
{
  localPlayerLocation = lpl;
  TheObjectManager::Instance()->SetLocalPlayerLocation(lpl);
}

bool Player::IsLocalPlayer() const
{
  return (m_id == localPlayerId);
}
    
Player* GetLocalPlayer()
{
  if (localPlayerId == -1)
  {
    return 0;
  }

  GameObject* p = TheObjectManager::Instance()->GetGameObject(localPlayerId);

  // May return 0
  return (Player*)p;
}

void SetLocalPlayerId(int id)
{
  localPlayerId = id;
}

int GetLocalPlayerId()
{
  return localPlayerId;
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


