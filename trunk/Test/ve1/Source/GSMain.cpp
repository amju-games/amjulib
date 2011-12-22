#include "GSMain.h"
#include <AmjuGL.h>
#include <Unproject.h>
#include <ClipLineSegBox.h>
#include <iostream>
#include <Game.h>
#include "Terrain.h"
#include "Ve1SceneGraph.h"
#include "LocalPlayer.h"
#include "Ve1OnlineReqManager.h"
#include <StringUtils.h>
#include "ObjectManager.h"
#include "ObjectUpdater.h"
#include <CursorManager.h>
#include "GSPaused.h"
#include <EventPoller.h>

namespace Amju
{
void OnPauseButton()
{
  TheGame::Instance()->SetCurrentState(TheGSPaused::Instance());
}

void OnBuildButton()
{
  // Enable build GUI
}

GSMain::GSMain()
{
  m_moveRequest = false;
}

void GSMain::ShowObjectMenu(GameObject* obj)
{
  m_menu = new GuiMenu;

  m_menu->SetPos(m_mouseScreen);

  Ve1Object* v = dynamic_cast<Ve1Object*>(obj);
  if (v)
  {
    v->SetMenu(m_menu);
  }

/*
  GuiMenu* childMenu = new GuiMenu;
  childMenu->SetName("Child menu");
  childMenu->AddItem(new GuiMenuItem("good"));
  childMenu->AddItem(new GuiMenuItem("Lord"));
  childMenu->AddItem(new GuiMenuItem("this"));
  childMenu->AddItem(new GuiMenuItem("seems"));
  childMenu->AddItem(new GuiMenuItem("to"));
  childMenu->AddItem(new GuiMenuItem("work"));

  m_menu = new GuiMenu;
  
  m_menu->AddItem(new GuiMenuItem("I am"));
  m_menu->AddItem(new GuiMenuItem("some text"));
  m_menu->AddItem(new GuiNestMenuItem("I R Nested!", childMenu));
*/
}

void GSMain::Update()
{
  GSBase::Update();

  // Make periodic checks for newly created objects
  TheObjectManager::Instance()->Update();

  TheObjectUpdater::Instance()->Update();

  TheGame::Instance()->UpdateGameObjects();
}

void GSMain::DoMoveRequest()
{
    Vec3f mouseWorldNear;
    Vec3f mouseWorldFar;

    Unproject(m_mouseScreen, 0, &mouseWorldNear);
    Unproject(m_mouseScreen, 1, &mouseWorldFar);
    LineSeg lineSeg(mouseWorldNear, mouseWorldFar);

/*
    // Draw for debugging
    std::cout << "Selecting, mouse: x: " << m_mouseScreen.x << " y: " << m_mouseScreen.y << "\n";
    AmjuGL::PushAttrib(AmjuGL::AMJU_TEXTURE_2D);
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
    AmjuGL::DrawLine(AmjuGL::Vec3(mouseWorldNear.x + 0.1f, mouseWorldNear.y + 0.1f, mouseWorldNear.z),
      AmjuGL::Vec3(mouseWorldFar.x, mouseWorldFar.y, mouseWorldFar.z));
    AmjuGL::PopAttrib();
*/

    GameObject* selectedObj = 0;
    GameObjects* objs = TheGame::Instance()->GetGameObjects();
    float bestDist = 999999.9f;
    for (GameObjects::iterator it = objs->begin(); it != objs->end(); ++it)
    {
      GameObject* pgo = it->second;
      Assert(pgo);
      AABB* aabb = pgo->GetAABB();
      if (aabb && Clip(lineSeg, *aabb, 0))
      {
        // Line seg intersects this box
        // Choose object whose centre (position) is closest to line seg..?
        float dist = LineSeg(mouseWorldNear, mouseWorldFar).SqDist(pgo->GetPos());
        //float dist = (mouseWorldNear - pgo->GetPos()).SqLen(); // pick closest
        if (dist < bestDist)
        {
          bestDist = dist;
          selectedObj = pgo;
        }
      }
    }
    if (selectedObj)
    {
      const std::string name = selectedObj->GetTypeName();
      std::cout << "Selected " << name << " ID: " << selectedObj->GetId() << "\n";

      ShowObjectMenu(selectedObj);
    }
    else if (GetLocalPlayer())
    {
std::cout << "Ground clicked...\n";
      Vec3f pos = Terrain::GetTerrain()->GetMousePos(lineSeg);
std::cout << "Pos: " << pos.x << ", " << pos.y << ", " << pos.z << "\n";

      TheObjectUpdater::Instance()->SendPosUpdateReq(GetLocalPlayer()->GetId(), pos);
    }
}

void GSMain::Draw()
{
/////  GSBase::Draw();
  AmjuGL::SetClearColour(Colour(1, 1, 0, 1));

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR_PLANE = 1.0f;
  const float FAR_PLANE = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR_PLANE, FAR_PLANE);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  // TODO Super simple camera
  static const float CAM_Y = 100.0f;
  static const float CAM_Z = 150.0f;

  if (GetLocalPlayer())
  {
    const Vec3f& pos = GetLocalPlayer()->GetPos();

//std::cout << "Player pos: " << pos.x << " " << pos.y << " " << pos.z << "\n";

    AmjuGL::LookAt(pos.x, pos.y + CAM_Y, pos.z + CAM_Z,  pos.x, pos.y, pos.z,  0, 1, 0);
  }
  else
  {
    AmjuGL::LookAt(0, CAM_Y, CAM_Z,  0, 0, 0,  0, 1, 0);
  }

  GetVe1SceneGraph()->Draw();

  if (m_moveRequest)
  {
    // Find point on ground clicked; create request and send to server
    DoMoveRequest();
    m_moveRequest = false;
  }
}

void GSMain::Draw2d()
{
  m_gui->Draw();

  if (m_menu)
  {
    m_menu->Draw();
  }

  TheCursorManager::Instance()->Draw();
}

void GSMain::OnDeactive()
{
  m_gui = 0;
  m_menu = 0;
}

void GSMain::OnActive()
{
  GSBase::OnActive();

  // We only want to do this if we decide to reload the world
//  GetVe1SceneGraph()->Clear();

  // Load HUD GUI
  m_gui = LoadGui("gui-hud.txt");
  Assert(m_gui);
  GuiElement* pauseButton = m_gui->GetElementByName("pause-button");
  Assert(pauseButton); // bad gui file..?
  pauseButton->SetCommand(Amju::OnPauseButton);
  //TheEventPoller::Instance()->SetListenerPriority(pauseButton, -1); // so we don't move to the button pos

  m_gui->GetElementByName("build-button")->SetCommand(Amju::OnBuildButton);
}

bool GSMain::OnCursorEvent(const CursorEvent& ce)
{
  m_mouseScreen.x = ce.x;
  m_mouseScreen.y = ce.y;

  return false;
}

bool GSMain::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
std::cout << "Mouse button event!! ";

  // Player has clicked somewhere on screen.
  // Response will depend on whether an object was clicked, or a GUI element.
  // If the ground was clicked, make a request to move to that location.

  // GUI Elements: these have higher priority so if we get here we have clicked on a 
  // GameObject or the ground

  if (!mbe.isDown)
  {
    m_moveRequest = true;
  }
  
/*
    if (m_menu && GetRect(m_menu).IsPointIn(m_mouseScreen))
    {
std::cout << "Click event - Get rid of menu\n";
      TheEventPoller::Instance()->RemoveListener(m_menu);
      m_menu = 0;
    }
  }
  else
  {
    if (!m_menu)
    {
std::cout << "Click event - move to new pos\n";
      // Move to location:
      m_moveRequest = true;
    }
  }
*/

  return false;
}

bool GSMain::OnKeyEvent(const KeyEvent& ke)
{
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 'u')
  {
    TheObjectUpdater::Instance()->Update();
    return true;
  }
  return false;
}

} // namespace
