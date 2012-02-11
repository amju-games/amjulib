#include "GSWaitForNewLocation.h"
#include <AmjuGL.h>
#include "Terrain.h"
#include "ObjectManager.h"
#include <Game.h>
#include "GSMain.h"

namespace Amju
{
GSWaitForNewLocation::GSWaitForNewLocation()
{
}

void GSWaitForNewLocation::Update()
{
  GSGui::Update();

  // Check for objects we are dependent on. When they are all present, we can go to main state.
  TheObjectManager::Instance()->Update();
  if (Terrain::GetTerrain())
  {
//    TheObjectManager::Instance()->GoToNewLocation();
    TheGame::Instance()->SetCurrentState(TheGSMain::Instance());
  }
}

void GSWaitForNewLocation::Draw()
{
  GSGui::Draw();

}

void GSWaitForNewLocation::Draw2d()
{
  GSGui::Draw2d();
}

void GSWaitForNewLocation::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-waitfornewlocation.txt");
  Assert(m_gui);
}

bool GSWaitForNewLocation::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSWaitForNewLocation::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
