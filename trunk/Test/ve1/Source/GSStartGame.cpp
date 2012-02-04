#include "GSStartGame.h"
#include "GSMain.h"
#include <AmjuGL.h>
#include <Game.h>
#include "Ve1SceneGraph.h"

namespace Amju
{
GSStartGame::GSStartGame()
{
}

void GSStartGame::Update()
{
  GSBase::Update();

}

void GSStartGame::Draw()
{
  GSBase::Draw();

}

void GSStartGame::Draw2d()
{
}

void GSStartGame::OnActive()
{
  GSBase::OnActive();

  // If single player, load objects now.

  // If multi player, start object manager

  // Once loaded..
  TheGame::Instance()->SetCurrentState(TheGSMain::Instance());

  // Clear scene graph and initialise empty root node
  ClearVe1SceneGraph();
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
