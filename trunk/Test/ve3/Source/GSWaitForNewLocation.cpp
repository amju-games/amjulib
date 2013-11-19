#include <AmjuFirst.h>
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
#include "Food.h"
#include "ROConfig.h"
#include "LoadLevel.h"
#include "Room.h"
#include "GSVe3HomePage.h"
#include <AmjuFinal.h>

//#define ANNOUNCE_ROOM_CHANGES

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
  m_setStartPos = false;
}
 
void GSWaitForNewLocation::SetStartPos(const Vec3f& pos)
{
  m_setStartPos = true;
  m_startPos = pos;
}

void GSWaitForNewLocation::SetLatestDownloadedFilename(const std::string& filename)
{
  if (!m_gui)
  {
    return; // no longer in this state, hmm..
  }
  GuiText* t = (GuiText*)GetElementByName(m_gui, "filename-text");
  Assert(t);
  t->SetText(filename);
}

void GSWaitForNewLocation::Update()
{
  GSGui::Update();

  // New June 2013: don't download anything. Load game objects from file.
  // But we still have to wait for player, sigh
  
  // Check for objects we are dependent on. When they are all present, we can go to main state.
  TheObjectManager::Instance()->Update();
  TheObjectUpdater::Instance()->Update();

  int localPlayerId = GetLocalPlayerId();
  int loc = GetLocalPlayerLocation();
  Ve1Object* playerObj = dynamic_cast<Ve1Object*>(TheObjectManager::Instance()->GetGameObject(localPlayerId).GetPtr());
  if (playerObj)
  {
    // Need to do this or we might wait forever here.
    // ObjectUpdater deliberately ignores pos updates for the local player,
    //  so why did this ever work? Race condition?
    playerObj->SetLocation(loc);
  
    if (m_setStartPos)
    {
      playerObj->SetPos(m_startPos);
      m_setStartPos = false;
    }
  }
  bool playerReady = (TheGame::Instance()->GetGameObject(localPlayerId) != 0);

#ifdef WHY_WAIT_DEBUG
std::cout << "Local player ID: " << localPlayerId << "\n";
if (playerObj && !playerReady)
{
  std::cout << "Player object loaded but not in game!?!\n";
}
#endif

  bool editMode = GetGameMode() == AMJU_MODE_EDIT;

  static int showStatus = ROConfig()->GetInt("new-location-show-status", 0);
  if (showStatus > 0)
  {
    std::cout //<< "Terrain is " << (terrainReady ? "READY" : "NOT ready") 
      << " player is " << (playerReady ? "READY" : "NOT ready") << "\n";
  }

  if (playerReady || editMode)
  {
    if (editMode)
    {
      TheGame::Instance()->SetCurrentState(TheGSEdit::Instance());
    }
    else
    {
      //TheGame::Instance()->SetCurrentState(TheGSVe3HomePage::Instance());

      
      Camera* cam = (Camera*)GetVe1SceneGraph()->GetCamera().GetPtr();
      cam->Reset();

      SetRandomFoodInLocation();

      // Hmm, this might clutter up the conversation too much.
#ifdef ANNOUNCE_ROOM_CHANGES
      std::string str;
      if (GetNameForPlayer(GetLocalPlayerId(), &str))
      {
        str += " entered room "; // TODO variety of msgs
        str += ToString(GetLocalPlayerLocation());
        TheMsgManager::Instance()->SendMsg(MsgManager::SYSTEM_SENDER, MsgManager::BROADCAST_RECIP, str);
      }
#endif

      static GSMain* gsm = TheGSMain::Instance();
      gsm->SetRoom(GetLocalPlayerLocation());
      TheGame::Instance()->SetCurrentState(gsm);
      
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

  TheObjectManager::Instance()->CheckForNewObjectsNow();
  TheObjectUpdater::Instance()->HintCheckForUpdates();

  // New June 2013: don't download anything. Load game objects from file.

  // What's our new location?
  int loc = GetLocalPlayerLocation();
  // Load level (room) for this level
  std::string filename = MakeRoomFilename(loc);
  
  // TODO Load one object at a time
  if (!LoadLevel(filename))
  {
    // Serious problem, can't load level
    Assert(0);
  }
}

} // namespace
