#include "GSEdit.h"
#include "Ve1SceneGraph.h"
#include <AmjuGL.h>
#include <CursorManager.h>
#include <Game.h>
#include "GSNetError.h"
#include "PickObject.h"
#include "ObjectManager.h"
#include "ObjectUpdater.h"
#include "MsgManager.h"
#include "LocalPlayer.h"
#include "GSNewLocation.h"
#include "GSStartMenu.h"
#include "Portal.h" // TODO hopefully we can query GameObjectFactory

namespace Amju
{
void OnDone()
{
  // TODO Any need to check for unsaved changes ? Everything is saved as we go along, right ?
  TheGame::Instance()->SetCurrentState(TheGSStartMenu::Instance());
}

GSEdit::GSEdit()
{
  m_mouseDownLeft = false;
  m_mouseDownRight = false;
  m_location = 0; // TODO Needs to sync with what GSStartGame does
}

int GSEdit::GetUniqueId()
{
  // Get ID for a new game object which is globally unique for the entire game.
  // Could query server. Or make one from time + MAC address etc ?

  return 0;
}

void GSEdit::OnNextLocation()
{
  m_location++;
  SetLocalPlayerLocation(m_location);
  // Will wait in GSWaitForNewLocation. Need to add a cancel button there, so we can come back here.
  // But need to reset to old location in this case..???
}

void GSEdit::Update()
{
  GSBase::Update();

  GetVe1SceneGraph()->Update();

  // TODO Update online req mgr if we are waiting for something, 
  //  e.g. a new ID or are uploading some content.

  // From GSMain: probably DON'T do these:
  TheObjectManager::Instance()->Update(); 
  TheObjectUpdater::Instance()->Update();
  //TheMsgManager::Instance()->Update();
  //TheGame::Instance()->UpdateGameObjects();
  //TheSAP::Instance()->Update(); // sweep & prune

}

void GSEdit::Draw()
{
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

  GetVe1SceneGraph()->Draw();

  if (m_mouseDownLeft)
  {
    // Check for selected object
std::cout << "PICKING!\n";
    GameObject* obj = PickObject(m_mouseScreen);
    if (obj)
    {
      // TODO Multi-select - use Shift and Ctrl like Maya
      m_selectedObjects.clear();
      if (obj)
      {
        Ve1Object* vobj = dynamic_cast<Ve1Object*>(obj);
        Assert(vobj);
        m_selectedObjects.insert(vobj);
      }
    }
    // Change text
    std::string selText;
    if (m_selectedObjects.empty())
    {
      selText = "no selection";
    }
    else if (m_selectedObjects.size() == 1)
    {
      selText = "one object selected";
    }
    else
    {
      selText = "multiple objects selected";
    }
    ((GuiText*)m_gui->GetElementByName("text1"))->SetText(selText);
  }
}

void GSEdit::Draw2d()
{
  m_gui->Draw();

  if (m_menu)
  {
    m_menu->Draw();
  }

  TheCursorManager::Instance()->Draw();

}

void GSEdit::OnActive()
{
  GSBase::OnActive();

  TheGSNetError::Instance()->SetPrevState(this);

  // We only want to do this if we decide to reload the world
//  GetVe1SceneGraph()->Clear();

  // Load HUD GUI
  m_gui = LoadGui("gui-edit.txt");
  Assert(m_gui);

  // Set commands for GUI buttons
  m_gui->GetElementByName("done-button")->SetCommand(Amju::OnDone);

  std::string loc = "Location: " + ToString(GetLocalPlayerLocation());
  ((GuiText*)m_gui->GetElementByName("location_info"))->SetText(loc);
}

void GSEdit::OnDeactive()
{
  GSBase::OnDeactive();
  m_gui = 0;
  m_menu = 0;

}

bool GSEdit::OnCursorEvent(const CursorEvent& ce)
{
  m_mouseScreen = Vec2f(ce.x, ce.y);
  return false;
}

bool GSEdit::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  m_mouseScreen = Vec2f(mbe.x, mbe.y);

  // Left click: check for clicking on an object
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    m_mouseDownLeft = mbe.isDown;
std::cout << "Left Mouse button event! " << (m_mouseDownLeft ? "DOWN" : "UP") << "\n";
  }
  else if (mbe.button == AMJU_BUTTON_MOUSE_RIGHT)
  {
    m_mouseDownRight = mbe.isDown;
    if (m_mouseDownRight)
    {
      // Create context menu
      CreateContextMenu();
    }
  }

  return false;
}

void OnNewLocation()
{
  TheGSNewLocation::Instance()->SetMode(GSNewLocation::AMJU_ADD_NEW);
  TheGame::Instance()->SetCurrentState(TheGSNewLocation::Instance());
}

void OnEditLocation()
{
  TheGSNewLocation::Instance()->SetMode(GSNewLocation::AMJU_EDIT);
  TheGame::Instance()->SetCurrentState(TheGSNewLocation::Instance());
}

class NewObjectCommand : public GuiCommand
{
public:
  NewObjectCommand(const std::string& typeName) : m_typeName(typeName) { }
  virtual bool Do()
  {
std::cout << "Create new object of type " << m_typeName << "\n";

    return false;
  }

private:
  std::string m_typeName;
};

void GSEdit::CreateContextMenu()
{
  m_menu = new GuiMenu;
  m_menu->SetPos(m_mouseScreen);
  if (m_selectedObjects.empty())
  {
    // 
    m_menu->AddItem(new GuiMenuItem("Add new Location...", OnNewLocation));
    m_menu->AddItem(new GuiMenuItem("Update this mesh...", OnEditLocation));

    GuiMenu* childMenu = new GuiMenu;
    childMenu->SetName("Child menu");

    // Get types from Game Object Factory..?
    childMenu->AddItem(new GuiMenuItem("Portal", new NewObjectCommand(Portal::TYPENAME)));
    // TODO Other types

    m_menu->AddItem(new GuiNestMenuItem("New Game Object...", childMenu));
  }
  else if (m_selectedObjects.size() == 1)
  {
  }
  else
  {
    // Multi selection
  }
}

bool GSEdit::OnKeyEvent(const KeyEvent&)
{
  return false;
}

}

