#include "GSStartGame.h"
#include "GSMain.h"
#include <AmjuGL.h>
#include <Game.h>
#include "LocalPlayer.h"

namespace Amju
{
GSStartGame::GSStartGame()
{
}

void GSStartGame::Update()
{
  GSBase::Update();

  //TheObjectManager::Instance()->Update(); // no need, we will be out of here next frame
}

void GSStartGame::Draw()
{
  GSBase::Draw();

}

void GSStartGame::Draw2d()
{
  // No need to draw, we will be changing to 'wait for location' state very soon..
}

void GSStartGame::OnActive()
{
  GSBase::OnActive();

  // If single player, load objects now.

  // If multi player, start object manager

  // Once loaded..
  //TheGame::Instance()->SetCurrentState(TheGSMain::Instance());

  // Clear scene graph and initialise empty root node
  //ClearVe1SceneGraph();

  // What is the starting location ? Do players always start at "home" location ?
  // Let's say they do always start at location 0
  int loc = 0;
  SetLocalPlayerLocation(loc);
}

bool GSStartGame::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSStartGame::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
