#include <AmjuFirst.h>
#include "GSMain.h"
#include <AmjuGL.h>
#include <CollisionManager.h>
#include <ShadowMap.h>
#include <Screen.h>
#include <SAP.h> 
#include <Unproject.h>
#include <ClipLineSegBox.h>
#include <iostream>
#include <Game.h>
#include <GuiText.h>
#include <Timer.h>
#include "Ve1Req.h"
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
#include <GuiButton.h>
#include <SoundManager.h>
#include "GSNetError.h"
#include "GameMode.h"
#include "Camera.h"
#include "PickObject.h"
#include "ChatConsole.h"
#include "MouseToGroundPos.h"
#include "HasCollisionMesh.h"
#include "Useful.h"
#include "LurkMsg.h"
#include "Furniture.h"
#include "CreateCollect.h" // TODO TEMP TEST
#include "Ve1BruteForce.h" // test against SAP
#include "FirstTimeMsg.h"
#include "Ve1Character.h"
#include "ROConfig.h"
#include "GSCogTestMenu.h"
#include "GameConsts.h"
#include "CogTestNag.h"
#include "GS2dAvatarMod.h"
#include "MsgNum.h"
#include "CommandPickup.h"
#include "BroadcastConsole.h"
#include "Kb.h"
#include <AmjuFinal.h>

#define SHOW_NUM_ERRORS
//#define USE_SHADOW_MAP

#define PICK_DEBUG

namespace Amju
{
void OnPauseButton()
{
  TheGSPaused::Instance()->SetPrevState(TheGSMain::Instance());
  TheGame::Instance()->SetCurrentState(TheGSPaused::Instance());
}

void OnQuitButton()
{
  TheGSMain::Instance()->OnQuitButton();
}

GSMain::GSMain()
{
  m_moveRequest = false;
  m_yRot = 0;
  m_listener = new GSMainListener;
  m_numPlayersOnline = 0;

  m_viewportWidth = 1.0f; 

  m_quitConfirm = false;

  m_menuObject = 0;
  m_wasClickedAway = false;
}

GuiComposite* GSMain::GetContextMenu()
{
#ifdef USE_BUTTONS_FOR_CONTEXT_MENU
  GuiComposite* contextMenu = dynamic_cast<GuiComposite*>(GetElementByName(m_gui, "context-menu"));
  Assert(contextMenu);
  return contextMenu;
#else
  return m_menu;
#endif
}

void GSMain::ClearContextMenu()
{
#ifdef USE_BUTTONS_FOR_CONTEXT_MENU
  // Send off screen ?
  if (m_gui)
  {
    GetContextMenu()->SetVisible(false);
  }
#else
  if (m_menu)
  {
    TheEventPoller::Instance()->RemoveListener(m_menu);
  }
  m_menu = 0;
#endif  
}

void GSMain::AddMenuItem(const std::string& text, GuiCommand* command)
{
  GuiComposite* contextMenu = GetContextMenu();
  if (!contextMenu)
  {
    return;
  }

#ifdef USE_BUTTONS_FOR_CONTEXT_MENU

  GuiButton* button = new GuiButton;
  bool loadedOk = button->OpenAndLoad("context-button.txt");
  Assert(loadedOk);
  button->SetText(text);
  button->SetCommand(command);
  // Set position depending on how many buttons are already there
  float y = (float)(contextMenu->GetNumChildren()) * button->GetSize().y;
  Vec2f pos(-1.0f, y);
  button->SetLocalPos(pos);
  contextMenu->AddChild(button);

#else
  contextMenu->AddChild(new GuiMenuItem(text, command));
#endif

}

void GSMain::ShowDropButton(Furniture* f, bool show)
{
  return; // Don't like, not required..?

std::cout << "Drop Button: show: " << show << " object: " << *f << "\n";

  if (!m_gui)
  {
    return; // TODO TEMP TEST - HACK, shouldn't get here
  }

  GuiButton* pickupComp = (GuiButton*)GetElementByName(m_gui, "pickup-comp");

  if (show)
  {
    GuiButton* drop = (GuiButton*)GetElementByName(m_gui, "drop-button");
    drop->SetCommand(new CommandPickUp(f, false));

    //GuiButton* rotate = (GuiButton*)GetElementByName(m_gui, "rotate-button");
    //rotate->SetCommand(new CommandRotate(f));

    pickupComp->SetVisible(true);
  }
  else
  {
    pickupComp->SetVisible(false);
  }
}

static int score = 0;

void GSMain::SetScore(int num)
{
  score = num;
}

void GSMain::ResetHud()
{
  score = 0;
}

void GSMain::SetNumPlayersOnline(int n)
{
  m_numPlayersOnline = n;

  // Rethink this. We generate msgs a lot faster than we display them :-(

  //std::string s = "Players online: " + ToString(m_numPlayersOnline);
  //TheLurker::Instance()->Queue(LurkMsg(s, Colour(1, 1, 1, 1), Colour(0.2, 0, 0.2, 1), AMJU_BOTTOM));
}

void OnMenuClickedAway()
{
  TheGSMain::Instance()->OnMenuClickedAway();
}

bool GSMain::ShowObjectMenu(GameObject* obj)
{
  // Get distance from local player to this object, don't show menu if too far away
  // TODO
  static const float MENU_DISTANCE = ROConfig()->GetFloat("menu-obj-distance", 100.0f); // TODO CONFIG ?
  static const float MENU_DISTANCE2 =  MENU_DISTANCE * MENU_DISTANCE;

  float sqdist = (GetLocalPlayer()->GetPos() - obj->GetPos()).SqLen();
  if (sqdist > MENU_DISTANCE2)
  {
    return false;
  }

  Ve1Object* v = dynamic_cast<Ve1Object*>(obj);
  if (v)
  {
    if (v == m_menuObject && m_wasClickedAway)
    {
      // Player previously clicked on this object and did not select a choice.
      // So don't show menu again, allowing player to select position on the object.
      return false;
    }

    //RCPtr<GuiMenu> menu = 0; // TODO TEMP TEST new GuiMenu;
    m_menu = new GuiMenu;
#ifdef _DEBUG
    m_menu->SetName("context menu for object " + ToString(*v));
#endif

    //GetContextMenu()->Clear();
    //GetContextMenu()->SetVisible(true);
    v->SetMenu(m_menu);

#ifdef WTF
    if (GetContextMenu()->GetNumChildren() > 0)
    {
      return true; // so don't walk to position clicked
    }
#endif
    // Only show menu if it has items
    
    if (m_menu->GetNumChildren() > 0)
    {
      m_menuObject = v;
      m_wasClickedAway = false;

      m_menu->SetOnClickedAwayFunc(Amju::OnMenuClickedAway);
      TheEventPoller::Instance()->AddListener(m_menu, -1); // cursor is -2 priority
      // TODO Fix this so we can see it if on right hand side
      Vec2f pos = m_mouseScreen;
      Vec2f size = m_menu->GetSize();
      if (pos.x + size.x > 1.0f)
      {
        pos.x = 1.0f - size.x;
      }
      m_menu->SetLocalPos(pos);
      return true;
    }
    else
    {
      m_menu = 0;
    }
    
  }
  return false;
}

static void OnChangeLookNo()
{
  // TODO Add this as an "on No" message, otherwise you get msgs shuffled with others.
//  FirstTimeMsgThisSession("OK, you can change later if you like, by clicking on the pause button.", UNIQUE_MSG_ID, false);
  FirstTimeMsgThisSession("To walk around, click on the ground where you want to go.", UNIQUE_MSG_ID, false);
}

static void OnChangeLookYes()
{
  TheGS2dAvatarMod::Instance()->SetPrevState(TheGSMain::Instance());
  TheGame::Instance()->SetCurrentState(TheGS2dAvatarMod::Instance());
  FirstTimeMsgThisSession("To walk around, click on the ground where you want to go.", UNIQUE_MSG_ID, false);
}

void GSMain::Update()
{
  GSBase::Update();

  std::string s;
  GameMode gm = GetGameMode();
  if (gm == AMJU_MODE_SINGLE)
  {
    // TODO localise
    s = "Hello, <p>! Today, you are hungry and must find food for yourself! Pick up and eat food by walking into it.";
  }
  else if (gm == AMJU_MODE_MULTI)
  {
    s = "Hello, <p>! Today, you must feed the other players! Pick up food by walking into it, then give it to other players by walking into them.";
  }
  else if (gm == AMJU_MODE_EDIT)
  {
    s = "Have fun in edit mode!";
  }
  else
  {
    Assert(0); // how did we get here?
  }

  FirstTimeMsgThisSession(
    s,
    UNIQUE_MSG_ID,
    false);

  FirstTimeQuestionThisSession(
    "Hmm... you look... a bit funny. Would you like to change the way you look ?", 
    UNIQUE_MSG_ID, OnChangeLookNo, OnChangeLookYes);

  // This is to nag about cog tests -- but if it's a test day, you shouldn't be
  // playing, right?
  //TheCogTestNag::Instance()->Update();

  // Disable pause button if Lurk msg showing
  GuiButton* pauseButton = (GuiButton*)m_gui->GetElementByName("pause-button");
  pauseButton->SetIsEnabled(!TheLurker::Instance()->IsDisplayingMsg());

  GetVe1SceneGraph()->Update();

  // Make periodic checks for newly created objects
  // Just do this in GSWaitForNewLocation, right?
  TheObjectManager::Instance()->Update();

  TheObjectUpdater::Instance()->Update();

  TheMsgManager::Instance()->Update();

  TheGame::Instance()->UpdateGameObjects();

  // TODO SAP instead
  TheCollisionManager::Instance()->InitFrame(); // TODO this should be in Game class
  BruteForce(TheGame::Instance()->GetGameObjects());

  if (GetGameMode() == AMJU_MODE_MULTI)
  {
    TheChatConsole::Instance()->Update();
    TheBroadcastConsole::Instance()->Update();
    TheKb::Instance()->Update();
  }

  TheLurker::Instance()->Update();

  // Update hearts and fuel cells
  GuiText* text1 = (GuiText*)GetElementByName(m_gui, "fuelcell-num");
  if (!text1) 
  {
    Assert(0);
std::cout << "SetFuelCells: no fuelcell-num element\n";
    return;
  }
  text1->SetText(ToString(score));
  text1->SetVisible(true); 

  // Simplify by removing one of the score elements
  /*
  GuiText* text2 = (GuiText*)GetElementByName(m_gui, "heart-num");
  if (!text2) 
  {
    Assert(0);
std::cout << "SetHeartNum: no heart-num element\n";
    return;
  }
  GuiElement* heartImg = GetElementByName(m_gui, "heart-img");
  Assert(heartImg);
  text2->SetText(ToString(hearts));
  text2->SetVisible(true); 
  heartImg->SetVisible(true);
  */

  /* // This was for flashing when health was <= 0
  if (hearts <= 0)
  {
    float t = TheTimer::Instance()->GetElapsedTime();
    t -= floor(t);
    Assert(t <= 1.0f);
    bool vis = (t < 0.5f);

    text2->SetVisible(vis);
    heartImg->SetVisible(vis);
  }
  else
  {
    text2->SetVisible(true); 
    heartImg->SetVisible(true);
  }
  */

}

void GSMain::DoMoveRequest()
{
  Vec2f mouseScreen = m_mouseScreen;

  // Transform x coord so -1 is at the left of the screen, +1 is the rightmost edge of the viewport.
  mouseScreen.x = (mouseScreen.x + 1.0f) * (1.0f / m_viewportWidth) - 1.0f;
    
  if (mouseScreen.x > 1.0f)
  {
    return;
  }
  // Convert y coord
  static BroadcastConsole* bc = TheBroadcastConsole::Instance();
  float bottom = -1.0f;
  if (GetGameMode() == AMJU_MODE_MULTI)
  {
    bottom = bc->GetY();
  }
  // Transform y so +1 is at the top of the screen, -1 is where the broadcast console starts.
  mouseScreen.y = (mouseScreen.y - bottom) / (1.0f - bottom) * 2.0f - 1.0f; 

  // Clicked below playable area
  if (mouseScreen.y < -1.0f)
  {
    return;
  }

#ifdef IPHONE
    // This rotate is for the Unproject calls later (in PickObject and GetMouseToGroundPos).
    // The mouse coord is rotated by the time we get here, so this rotates it back.
    
//    std::cout << "Before: ms.x: " << mouseScreen.x << ", ms.y: " << mouseScreen.y << "\n";
    std::swap(mouseScreen.y, mouseScreen.x);
    mouseScreen.x = -mouseScreen.x;
//    std::cout << "After: ms.x: " << mouseScreen.x << ", ms.y: " << mouseScreen.y << "\n";
#endif
        
  GameObject* selectedObj = PickObject(mouseScreen);

  if (selectedObj)
  {
#ifdef PICK_DEBUG
std::cout << "Selected " << *selectedObj << "\n";
#endif

    if (ShowObjectMenu(selectedObj))
    {
      return;
    }
    // No menu. Find point on mesh, and go to that position.

    if (selectedObj != m_menuObject)
    {
      m_menuObject = 0;
    }
  }

  // No object selected, so find position on terrain. 
  if (GetLocalPlayer())
  {
    Vec3f pos;

    // Try to treat selected object like terrain, and get a position to go to.
    bool gotpos = false;
    HasCollisionMesh* hcm = dynamic_cast<HasCollisionMesh*>(selectedObj);
    CollisionMesh* collMesh = hcm ? hcm->GetCollisionMesh() : 0;
    if (hcm && collMesh)
    {
#ifdef PICK_DEBUG
std::cout << "Try to find point on this object: " << *selectedObj << "\n";
#endif

      gotpos = MouseToGroundPos(collMesh, mouseScreen, &pos);
      if (gotpos)
      {
#ifdef PICK_DEBUG
std::cout << " - Found point on ground on this obj: " << *selectedObj << "!\n";
#endif
      }
      else
      {
#ifdef PICK_DEBUG
std::cout << " - No luck!\n";
#endif
      }
    }
    else if (selectedObj)
    {
#ifdef PICK_DEBUG
std::cout << *selectedObj << " does not have collision mesh.\n";
#endif
    }

    if (!gotpos)
    {
#ifdef PICK_DEBUG
std::cout << "Try to find point on terrain for this mouse pos.\n";
#endif
      gotpos = true;   //MouseToGroundPos(GetTerrain()->GetCollisionMesh(), mouseScreen, &pos);
      pos = MouseToXZ(mouseScreen);
      m_menuObject = 0; // so we can now select an object again
    }

    if (gotpos)
    {
#ifdef PICK_DEBUG
std::cout << "Pos: " << pos.x << ", " << pos.y << ", " << pos.z << "\n";
#endif
      Player* player = GetLocalPlayer();

      // 2D game: assume ground is a flat plane
      pos.y = player->GetPos().y;

      // Sanity check the destination: if too far away, discard, it was probably not what the
      //  player intended.
      float sqDist = (player->GetPos() - pos).SqLen();
#ifdef PICK_DEBUG
std::cout << "Sq dist to arrow pos is: " << sqDist << "\n";
#endif
      static const float MAX_DIST = ROConfig()->GetFloat("max-click-dist", 2000.0f); 
      const float MAX_SQ_DIST = MAX_DIST * MAX_DIST; 
      if (sqDist < MAX_SQ_DIST)
      { 
        int location = GetLocalPlayerLocation(); // It's the current location, unless we hit a portal.
         // TODO Not sure how this is going to work. Do we detect a portal collision client-side ?
         // Maybe don't send location, but send it as a separate kind of request ?

        // TODO We want to respond immediately but we get out of sync with server
        // Move towards point, but server will send back actual destination
        player->SetArrowPos(pos); 
        player->SetArrowVis(true);

        TheObjectUpdater::Instance()->SendPosUpdateReq(GetLocalPlayer()->GetId(), pos, location);
        player->MoveTo(pos); // client side predict - respond immediately
      }
    }
    else
    {
#ifdef PICK_DEBUG
std::cout << "...not a point on the ground, apparently..?\n";
#endif
    }
  }
}

void GSMain::SetViewWidth(float w)
{
  m_viewportWidth = w;
}

#ifdef USE_SHADOW_MAP
static void ShadowDraw()
{
  GetVe1SceneGraph()->Draw();
}
#endif

void GSMain::Draw()
{
//  AmjuGL::ReportState(std::cout);

  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));

  int width = (int)((float)Screen::X()  * m_viewportWidth);
  static BroadcastConsole* bc = TheBroadcastConsole::Instance();
  int y = 0;
  if (GetGameMode() == AMJU_MODE_MULTI)
  {
    y = (int)((bc->GetY() + 1.0f) * 0.5f * (float)Screen::Y());
  }
  AmjuGL::Viewport(0, y, width, Screen::Y() - y);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR_PLANE = 1.0f;
  const float FAR_PLANE = 20000.0f;
  float aspect = (float)width / (float)Screen::Y();
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR_PLANE, FAR_PLANE);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  Camera* cam = (Camera*)GetVe1SceneGraph()->GetCamera().GetPtr();
  cam->SetTarget(GetLocalPlayer()); // could be 0

#ifdef USE_SHADOW_MAP

  static PShadowMap sm = 0;
  if (!sm)
  {
    sm = AmjuGL::CreateShadowMap();
    sm->SetLightPos(AmjuGL::Vec3(20, 20, 20));
    sm->Init();
    sm->SetDrawFunc(ShadowDraw);
  }
  sm->Draw();

#else // USE_SHADOW_MAP

  GetVe1SceneGraph()->Draw();

#endif // USE_SHADOW_MAP

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

  static Kb* kb = TheKb::Instance();
  if (GetGameMode() == AMJU_MODE_MULTI)
  {
    TheBroadcastConsole::Instance()->Draw();
    kb->Draw();
    // TODO Maybe we don't need this
    TheChatConsole::Instance()->Draw();
  }

  if (m_gui)
  {
    m_gui->Draw();
  }

  if (m_menu)
  {
    m_menu->Draw();
  }

  TheLurker::Instance()->Draw();

#ifdef SHOW_NUM_ERRORS
  static int showErrs = ROConfig()->GetInt("show-errors", 0);
  if (showErrs > 0)
  {
    // Show number of critical/non critical errors from server 
    static GuiText t;
    int critical = 0;
    int nonCritical = 0;
    Ve1Req::GetNumErrors(&critical, &nonCritical);
    // Get serial request queue size
    int q = TheVe1ReqManager::Instance()->CountAllReqs();
    t.SetSize(Vec2f(1.0f, 0.1f));
    t.SetJust(GuiText::AMJU_JUST_LEFT);
    t.SetDrawBg(true);
    t.SetLocalPos(Vec2f(-1.0f, 1.0f));
    std::string s = "Errs CR:" + ToString(critical) + " NC:" + ToString(nonCritical) + " Q: " + ToString(q);
    t.SetText(s);
    t.Draw();
  }
#endif

  TheCursorManager::Instance()->Draw();
}

void GSMain::OnMenuClickedAway()
{
std::cout << "Clicked off menu, so now it's invisible.\n";
  ClearContextMenu();

  m_wasClickedAway = true;
}

void GSMain::OnDeactive()
{
  GSBase::OnDeactive();

  if (TheEventPoller::Instance()->HasListener(m_gui))
  {
    TheEventPoller::Instance()->RemoveListener(m_gui);
  }
  m_gui = 0;

  // TODO Poss leaking menus ?? Check EventListeners
  m_menu = 0;

  TheChatConsole::Instance()->OnDeactive();
  TheKb::Instance()->Deactivate();

  TheEventPoller::Instance()->RemoveListener(m_listener);
}

void GSMain::OnActive()
{
#ifdef PLAY_MUSIC
  // TODO This should be per-location and also depend on time of day!
  TheSoundManager::Instance()->PlaySong("Sound/apz2.it");
#endif

  static const bool showAABB = (ROConfig()->GetValue("show-aabb", "n") == "y");
  SceneNode::SetGlobalShowAABB(showAABB);

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

  GuiButton* quitButton = (GuiButton*)m_gui->GetElementByName("quit-button");
  quitButton->SetCommand(Amju::OnQuitButton);
  
  GuiButton* drop = (GuiButton*)GetElementByName(m_gui, "pickup-comp");
  drop->SetVisible(false); // hide drop button until we pick someting up

  if (GetGameMode() == AMJU_MODE_MULTI)
  {
    TheChatConsole::Instance()->OnActive();
    TheBroadcastConsole::Instance()->OnActive();
    TheKb::Instance()->Activate();
  }

  TheEventPoller::Instance()->AddListener(m_listener, 100); 
  // high number = low priority, so GUI button clicks etc eat the events.
}

void GSMain::OnQuitButton()
{
  if (m_quitConfirm)
  {
    exit(0);
  }

  m_quitConfirm = true;
  GuiButton* quit = (GuiButton*)m_gui->GetElementByName("quit-button");
  quit->SetText("sure?");
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

bool GSMainListener::OnKeyEvent(const KeyEvent& kb)
{
#ifdef _DEBUG
  // TODO no good: broadcast edit box gets these events now.
  // Debug: show AABBs for SceneNodes
  if (kb.keyType == AMJU_KEY_CHAR && kb.key == 'b' && kb.keyDown)
  {
    static bool show = false;
    show = !show;
    SceneNode::SetGlobalShowAABB(show);
    return false;
  }
#endif

  return false;
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
std::cout << " - Chat active but NOT discarding mouse click\n";
    //return false; 
  }

  m_mouseScreen.x = mbe.x;
  m_mouseScreen.y = mbe.y;
  static Vec2f mouseScreenButtonDown;
  if (mbe.isDown)
  {
    mouseScreenButtonDown = m_mouseScreen;
  }

  float sqlen = (m_mouseScreen - mouseScreenButtonDown).SqLen();
#ifdef DRAG_DEBUG
  std::cout << "Sqlen: " << sqlen << "\n";
#endif
  static const float DRAG_MIN = ROConfig()->GetFloat("cam-drag-min", 0.05f);
  static const float DRAG_MIN2 = DRAG_MIN * DRAG_MIN;
  bool dragged = (sqlen > DRAG_MIN2);

  // Do req if mouse button is up, no drag, and no menu
  // TODO And we didn't just close the menu because we made a selection
#ifdef IPHONE
  if (!dragged && (!m_menu || !m_menu->IsVisible()))
  {
    m_moveRequest = true;
  }
    
#else
  if (!mbe.isDown && !dragged && (!m_menu || !m_menu->IsVisible()))
  {
    m_moveRequest = true;
  }
#endif
    
  if (m_quitConfirm)
  {
    m_quitConfirm = false; // clicked off quit button
    GuiButton* quit = (GuiButton*)m_gui->GetElementByName("quit-button");
    quit->SetText("quit");
  }

  return false;
}

} // namespace
