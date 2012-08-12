#include "GSWaitForNewLocation.h"
#include <AmjuGL.h>
#include "Terrain.h"
#include "ObjectManager.h"
#include "ObjectUpdater.h" // TODO This is stupid, just have one object
#include <Game.h>
#include "GSMain.h"
#include "GSEdit.h"
#include "GameMode.h"
#include "LocalPlayer.h"
#include "GSPaused.h"
#include "Ve1SceneGraph.h"
#include "Camera.h"

namespace Amju
{
static void OnCancel()
{
  if (GetGameMode() == AMJU_MODE_EDIT)
  {
    TheGame::Instance()->SetCurrentState(TheGSEdit::Instance());
  }
  else
  {
    // Set prev state for Pause
    TheGSPaused::Instance()->SetPrevState(TheGSWaitForNewLocation::Instance());
    TheGame::Instance()->SetCurrentState(TheGSPaused::Instance());
  }
}

GSWaitForNewLocation::GSWaitForNewLocation()
{
}

void GSWaitForNewLocation::Update()
{
  GSGui::Update();

  // Check for objects we are dependent on. When they are all present, we can go to main state.
  TheObjectManager::Instance()->Update();
  TheObjectUpdater::Instance()->Update();

  if (TerrainReady() && 
      TheGame::Instance()->GetGameObject(GetLocalPlayerId()))
  {
    if (GetGameMode() == AMJU_MODE_EDIT)
    {
      TheGame::Instance()->SetCurrentState(TheGSEdit::Instance());
    }
    else
    {
      Camera* cam = (Camera*)GetVe1SceneGraph()->GetCamera().GetPtr();
      cam->Reset();

      TheGame::Instance()->SetCurrentState(TheGSMain::Instance());
    }
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
  GetElementByName(m_gui, "cancel-button")->SetCommand(OnCancel);
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
