#include "GSStartGame.h"
#include "GSMain.h"
#include <AmjuGL.h>
#include <Game.h>
#include "LocalPlayer.h"

namespace Amju
{
GSStartGame::GSStartGame()
{
  m_startLoc = 1;
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

  SetLocalPlayerLocation(-1); // Force reload if m_startLoc is last location
  SetLocalPlayerLocation(m_startLoc);
  m_startLoc = 1; // for next time TODO Should this be -1 ??
}
} // namespace
