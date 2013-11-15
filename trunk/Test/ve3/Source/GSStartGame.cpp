#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Game.h>
#include "GSStartGame.h"
#include "GSMain.h"
#include "LocalPlayer.h"
#include "ObjectManager.h"
#include "ObjectUpdater.h"
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

std::cout << "GSStartGame: m_startLoc is: " << m_startLoc << "\n";

  if (m_startLoc != -1)
  {
    SetLocalPlayerLocation(m_startLoc);
  }
}
} // namespace
