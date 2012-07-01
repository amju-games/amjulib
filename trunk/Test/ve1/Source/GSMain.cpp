#include "GSMain.h"
#include <AmjuGL.h>
#include <Screen.h>
#include <SAP.h> 
#include <BruteForce.h> // test against SAP
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
#include "MsgManager.h"
#include <GuiTextEdit.h>
#include "GSNetError.h"
#include "GameMode.h"
#include "Camera.h"
#include "PickObject.h"
#include "ChatConsole.h"

namespace Amju
{
void OnPauseButton()
{
  TheGSPaused::Instance()->SetPrevState(TheGSMain::Instance());
  TheGame::Instance()->SetCurrentState(TheGSPaused::Instance());
}

GSMain::GSMain()
{
  m_moveRequest = false;
  m_yRot = 0;
  m_listener = new GSMainListener;
  m_numPlayersOnline = 0;

  m_viewportWidth = 1.0f; 
}

void GSMain::SetNumPlayersOnline(int n)
{
  m_numPlayersOnline = n;

std::cout << "Num players online is now: " << m_numPlayersOnline << "\n";
}

void GSMain::ShowObjectMenu(GameObject* obj)
{
  m_menu = new GuiMenu;
  TheEventPoller::Instance()->AddListener(m_menu);

  m_menu->SetLocalPos(m_mouseScreen);

  Ve1Object* v = dynamic_cast<Ve1Object*>(obj);
  if (v)
  {
    v->SetMenu(m_menu);
  }
}

void GSMain::Update()
{
  GSBase::Update();

  GetVe1SceneGraph()->Update();

  // Make periodic checks for newly created objects
  TheObjectManager::Instance()->Update();

  TheObjectUpdater::Instance()->Update();

  TheMsgManager::Instance()->Update();

  TheGame::Instance()->UpdateGameObjects();

  // TODO Need to set collision test function
//  TheSAP::Instance()->Update(*(TheGame::Instance()->GetGameObjects())); // sweep & prune
  // TODO TEMP TEST
  BruteForce(TheGame::Instance()->GetGameObjects());

  TheChatConsole::Instance()->Update();
}

void GSMain::DoMoveRequest()
{
//std::cout << "In DoMoveRequest...\n";

  Vec2f mouseScreen = m_mouseScreen;
  mouseScreen.x = (mouseScreen.x + 1.0f) * (1.0f / m_viewportWidth) - 1.0f;
std::cout << "Mousescreen.x = " << mouseScreen.x << "\n";
  if (mouseScreen.x > 1.0f)
  {
    return;
  }

  GameObject* selectedObj = PickObject(mouseScreen);

  if (selectedObj)
  {
    const std::string name = selectedObj->GetTypeName();
//std::cout << "Selected " << name << " ID: " << selectedObj->GetId() << "\n";

    ShowObjectMenu(selectedObj);
  }
  else if (GetLocalPlayer())
  {
//std::cout << "Ground clicked...\n";
    Vec3f pos;
    Vec3f mouseWorldNear;
    Vec3f mouseWorldFar;
    Unproject(mouseScreen, 0, &mouseWorldNear);
    Unproject(mouseScreen, 1, &mouseWorldFar);
    LineSeg lineSeg(mouseWorldNear, mouseWorldFar);

    if (GetTerrain()->GetMousePos(lineSeg, &pos))
    {
//std::cout << "Pos: " << pos.x << ", " << pos.y << ", " << pos.z << "\n";

      int location = GetLocalPlayerLocation(); // It's the current location, unless we hit a portal.
       // TODO Not sure how this is going to work. Do we detect a portal collision client-side ?
       // Maybe don't send location, but send it as a separate kind of request ?

      // TODO We want to respond immediately but we get out of sync with server
      // Move towards point, but server will send back actual destination
      GetLocalPlayer()->SetArrowPos(pos); 
      GetLocalPlayer()->SetArrowVis(true);

      TheObjectUpdater::Instance()->SendPosUpdateReq(GetLocalPlayer()->GetId(), pos, location);
      GetLocalPlayer()->MoveTo(pos); // client side predict - respond immediately

    }
    else
    {
//std::cout << "...not a point on the ground, apparently..?\n";
    }
  }
}

void GSMain::SetViewWidth(float w)
{
  m_viewportWidth = w;
}

void GSMain::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));

  int width = (int)((float)Screen::X()  * m_viewportWidth);
  AmjuGL::Viewport(0, 0, width, Screen::Y());

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR_PLANE = 1.0f;
  const float FAR_PLANE = 3000.0f;
  float aspect = (float)width / (float)Screen::Y();
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR_PLANE, FAR_PLANE);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  Camera* cam = (Camera*)GetVe1SceneGraph()->GetCamera().GetPtr();
  cam->SetTarget(GetLocalPlayer()); // could be 0

  GetVe1SceneGraph()->Draw();

  if (m_moveRequest)
  {
    // Find point on ground clicked; create request and send to server
    AmjuGL::PushMatrix();
    // Camera has been popped
    cam->Draw();
    DoMoveRequest();
    m_moveRequest = false;
    AmjuGL::PopMatrix();
  }
  
  AmjuGL::Viewport(0, 0, Screen::X(), Screen::Y());
}

void GSMain::Draw2d()
{
  GSBase::Draw2d();

  m_gui->Draw();

  if (m_menu)
  {
    m_menu->Draw();
  }

  TheChatConsole::Instance()->Draw();

  TheCursorManager::Instance()->Draw();
}

void GSMain::OnDeactive()
{
  GSBase::OnDeactive();

  m_gui = 0;

  // TODO Poss leaking menus ?? Check EventListeners
  m_menu = 0;

  TheChatConsole::Instance()->OnDeactive();

  TheEventPoller::Instance()->RemoveListener(m_listener);
}

void GSMain::OnActive()
{
  GSBase::OnActive();

  TheGSNetError::Instance()->SetPrevState(this);

  // We only want to do this if we decide to reload the world
//  GetVe1SceneGraph()->Clear();

  // Load HUD GUI
  m_gui = LoadGui("gui-hud.txt");
  Assert(m_gui);
  GuiElement* pauseButton = m_gui->GetElementByName("pause-button");
  Assert(pauseButton); // bad gui file..?
  pauseButton->SetCommand(Amju::OnPauseButton);
  //TheEventPoller::Instance()->SetListenerPriority(pauseButton, -1); // so we don't move to the button pos

  ////GetElementByName(m_gui, "build-button")->SetCommand(Amju::OnBuildButton);

  TheChatConsole::Instance()->OnActive();

  TheEventPoller::Instance()->AddListener(m_listener, 100); 
  // high number = low priority, so GUI button clicks etc eat the events.
}

static bool rightButtonDown = false;

bool GSMainListener::OnCursorEvent(const CursorEvent& ce)
{
  return TheGSMain::Instance()->OnCursorEvent(ce);
}

bool GSMainListener::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return TheGSMain::Instance()->OnMouseButtonEvent(mbe);
}

bool GSMain::OnCursorEvent(const CursorEvent& ce)
{
  static float oldx = ce.x;
  static float oldy = ce.y;
  float diffx = ce.x - oldx;
  //float diffy = ce.y - oldy; // not used yet
  oldx = ce.x;
  oldy = ce.y;

  if (rightButtonDown)
  {
    m_yRot += diffx * 10.0f;
  }

  m_mouseScreen.x = ce.x;
  m_mouseScreen.y = ce.y;

  return false;
}

bool GSMain::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
//std::cout << "Mouse button event!!\n";

  // Player has clicked somewhere on screen.
  // Response will depend on whether an object was clicked, or a GUI element.
  // If the ground was clicked, make a request to move to that location.

  // GUI Elements: these have higher priority so if we get here we have clicked on a 
  // GameObject or the ground
  // TODO Only ignore if cursor is in chat gui region
  if (mbe.button == AMJU_BUTTON_MOUSE_RIGHT)
  {
    rightButtonDown = mbe.isDown;
    return true;
  }

  if (TheChatConsole::Instance()->IsActive())
  {
std::cout << " - Chat active so discarding mouse click\n";
    return false; 
  }

  if (!mbe.isDown)
  {
    m_moveRequest = true;
  }
  
  return false;
}

} // namespace
