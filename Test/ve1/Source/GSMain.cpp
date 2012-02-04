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
#include "MsgManager.h"
#include <GuiTextEdit.h>
#include "GSNetError.h"

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

void OnChatSendButton()
{
  TheGSMain::Instance()->OnChatSend();
}

void OnChatCancelButton()
{
  TheGSMain::Instance()->OnChatCancel();
}

void OnRecvCloseButton()
{
  TheGSMain::Instance()->OnRecvClose();
}


void GSMain::OnChatSend()
{
  Assert(m_lastRecipId != -1);
  Assert(GetLocalPlayer()); // or couldn't send, right ?

  int senderId = GetLocalPlayer()->GetId();
  std::string text = 
    ((GuiTextEdit*)m_gui->GetElementByName("chat-text-edit"))->GetText();
  TheMsgManager::Instance()->SendMsg(senderId, m_lastRecipId, text);

  ActivateChatSend(false, -1); // ??
}

void GSMain::OnChatCancel()
{
  ActivateChatSend(false, -1); 
}

GSMain::GSMain()
{
  m_moveRequest = false;
  m_lastRecipId = -1;
  m_chatSendIsActive = false;
  m_chatRecvIsActive = false;
  m_yRot = 0;
}

bool GSMain::CanShowMsg() const
{
  return !m_chatRecvIsActive; // && !m_chatSendIsActive ??
}

//void GSMain::ShowMsg(const MsgManager::Msg& msg)
//{
//  ActivateChatRecv(true);
//  ((GuiText*)m_gui->GetElementByName("msg-recv-sender"))->SetText("TODO Get sender name");
//  ((GuiText*)m_gui->GetElementByName("msg-recv-text"))->SetText(msg.m_text);
//}

void GSMain::OnRecvClose()
{
  ActivateChatRecv(false);
}

void GetNameForPlayer(int objId, std::string* r)
{
  Player* p = dynamic_cast<Player*>(TheGame::Instance()->GetGameObject(objId).GetPtr());
  Assert(p);
  if (p)
  {
    *r = p->GetName();
  }
}

void GSMain::ActivateChatSend(bool active, int recipId)
{
  // TODO polish -- jump onto screen
  m_lastRecipId = recipId;

  if (active)
  {
std::cout << "Activate chat -- recip ID = " << recipId << "\n";

    std::string recipName;
    GetNameForPlayer(recipId, &recipName);

    m_gui->GetElementByName("chat-comp")->SetVisible(true);
    ((GuiText*)m_gui->GetElementByName("chat-recip-name"))->SetText(recipName);
    ((GuiTextEdit*)m_gui->GetElementByName("chat-text-edit"))->SetText("");
  }
  else
  {
    m_gui->GetElementByName("chat-comp")->SetVisible(false);
  }
  m_chatSendIsActive = active;
}

void GSMain::ActivateChatRecv(bool active, const MsgManager::Msg* msg)
{
  if (active)
  {
    std::string senderName;
    GetNameForPlayer(msg->m_senderId, &senderName);
    
    ((GuiText*)m_gui->GetElementByName("msg-recv-sender"))->SetText(senderName);
    ((GuiText*)m_gui->GetElementByName("msg-recv-text"))->SetText(msg->m_text);
  }

  m_gui->GetElementByName("msg-recv-comp")->SetVisible(active);
  m_chatRecvIsActive = active;
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

  TheMsgManager::Instance()->Update();

  TheGame::Instance()->UpdateGameObjects();

//  TheSAP::Instance()->Update(); // sweep & prune
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

      int location = 0; // TODO It's the current location, unless we hit a portal.
       // TODO Not sure how this is going to work. Do we detect a portal collision client-side ?
       // Maybe don't send location, but send it as a separate kind of request ?

      TheObjectUpdater::Instance()->SendPosUpdateReq(GetLocalPlayer()->GetId(), pos, location);

      // Move towards point, but server will send back actual destination
      GetLocalPlayer()->MoveTo(pos);
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

  PSceneNodeCamera cam = GetVe1SceneGraph()->GetCamera();

  if (GetLocalPlayer())
  {
    const Vec3f& pos = GetLocalPlayer()->GetPos();

//std::cout << "Player pos: " << pos.x << " " << pos.y << " " << pos.z << "\n";

    cam->SetEyePos(Vec3f(pos.x, pos.y + CAM_Y, pos.z + CAM_Z));
    cam->SetLookAtPos(pos);
  }
  else
  {
    cam->SetEyePos(Vec3f(0, CAM_Y, CAM_Z));
    cam->SetLookAtPos(Vec3f(0, 0, 0));
  }

//  Matrix m;
//  m.RotateY(m_yRot);
//  AmjuGL::MultMatrix(m);


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

  m_gui->GetElementByName("build-button")->SetCommand(Amju::OnBuildButton);

  m_gui->GetElementByName("chat-send-button")->SetCommand(Amju::OnChatSendButton);
  m_gui->GetElementByName("chat-cancel-button")->SetCommand(Amju::OnChatCancelButton);
  m_gui->GetElementByName("recv-close-button")->SetCommand(Amju::OnRecvCloseButton);
  ActivateChatSend(false, -1);
  ActivateChatRecv(false);
}

static bool rightButtonDown = false;

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
std::cout << "Mouse button event!! ";

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

  if (m_chatSendIsActive || m_chatRecvIsActive)
  {
std::cout << "Chat active so discarding mouse click\n";
    return false; 
  }

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
  return false;
}

} // namespace
