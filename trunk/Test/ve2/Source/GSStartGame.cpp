#include <AmjuFirst.h>
#include "GSStartGame.h"
#include "GSMain.h"
#include <AmjuGL.h>
#include <Game.h>
#include "LocalPlayer.h"
#include <AmjuFinal.h>

namespace Amju
{
GSStartGame::GSStartGame()
{
  m_startLoc = 1;
}

void GSStartGame::Update()
{
  GSGui::Update();

  // no need, we will be out of here next frame
  //TheObjectManager::Instance()->Update(); 
}

void GSStartGame::Draw()
{
  GSGui::Draw();
}

void GSStartGame::Draw2d()
{
  GSGui::Draw2d();
}

void GSStartGame::OnActive()
{
  GSGui::OnActive();

  if (m_startLoc != -1)
  {
    SetLocalPlayerLocation(-1); // Force reload if m_startLoc is last location
    SetLocalPlayerLocation(m_startLoc);
    m_startLoc = -1; 
  }
}
} // namespace
