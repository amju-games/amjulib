#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <Game.h>
#include "GSStartGame.h"
#include "GSMain.h"
#include "LocalPlayer.h"
#include "ObjectManager.h"
#include "ObjectUpdater.h"
#include "GSVe3GetObjects.h"
#include "Ve1SceneGraph.h"
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

  TheGame::Instance()->SetCurrentState(TheGSVe3GetObjects::Instance());
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

  // As we are not setting player location, we need to initialise the scene graph root
  ClearVe1SceneGraph();

  if (m_startLoc != -1)
  {
    // This call ends up changing the state to GSWaitForNewLocation, at end of
    //  ObjectManager::SetLocalPlayerLocation
    // -- Do it later when we "Explore"
    //SetLocalPlayerLocation(m_startLoc);
  }
}
} // namespace
