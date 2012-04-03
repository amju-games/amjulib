#include "GSEdit.h"
#include "Ve1SceneGraph.h"
#include <AmjuGL.h>
#include <CursorManager.h>
#include <Game.h>
#include <SafeUtils.h>
#include <GameObjectFactory.h>
#include <GuiListBox.h>
#include "GSNetError.h"
#include "PickObject.h"
#include "ObjectManager.h"
#include "ObjectUpdater.h"
#include "MsgManager.h"
#include "LocalPlayer.h"
#include "GSNewLocation.h"
#include "GSStartMenu.h"
#include "Portal.h" // TODO hopefully we can query GameObjectFactory
#include "ProtoObject.h"

namespace Amju
{
void OnDone()
{
  // TODO Any need to check for unsaved changes ? Everything is saved as we go along, right ?
  TheGame::Instance()->SetCurrentState(TheGSStartMenu::Instance());
}

class GetLocations : public Ve1Req
{
public:
  GetLocations(const std::string& url) : Ve1Req(url, "get locations") { }

  virtual void OnSuccess()
  {
    PXml p = m_xml.getChildNode(1);
    if (SafeStrCmp(p.getName(), "locs"))
    {
#ifdef XML_DEBUG
std::cout << "Found locs element\n";
#endif
  
      int num = p.nChildNode();

      Locs locs;
      locs.reserve(num);

      for (int i = 0; i < num; i++)
      {
        PXml child = p.getChildNode(i);
        if (!SafeStrCmp(child.getName(), "loc"))
        {
std::cout << "Got 'locs' but no 'loc' child...\n";
          OnFailure();
        }
        // Children of this 'loc' node: 'id' and 'name'
       
        int n = child.nChildNode();
        if (n != 2)
        {
std::cout << "Expected 2 children of 'loc' node!\n";
          OnFailure();
        } 
        int id = ToInt(child.getChildNode(0).getText());
        std::string name = child.getChildNode(1).getText();

std::cout << "Got new location: " << id << name << "\n";
        locs.push_back(Loc(id, name));
      }

std::cout << "Setting new location list.\n";
      TheGSEdit::Instance()->SetLocs(locs);
    } 
    else
    {
std::cout << "Get locations: expected 'locs' element\n";
      OnFailure();
    }  
  }
};

class ReqNewObjId : public Ve1Req
{
public:
  ReqNewObjId(const std::string& url, const std::string& type) : 
    Ve1Req(url, "new object id"), m_typename(type) { }

  virtual void OnSuccess()
  {
    PXml p = m_xml.getChildNode(1);
    if (SafeStrCmp(p.getName(), "id"))
    {
#ifdef XML_DEBUG
std::cout << "Found id element\n";
#endif

      std::string id = p.getText();
std::cout << "Got new object ID from server! " << id << "\n";

      ProtoObject* go = new ProtoObject; //TheGameObjectFactory::Instance()->Create(m_typename);
      Assert(go); // Typename should be from Factory, so Create should always succeed, right ?
      if (go)
      {
std::cout << "Success! Created object of type " << m_typename << "\n";
        go->SetId(ToInt(id));

        // TODO Set location and pos
        go->SetProtoLocation(GetLocalPlayerLocation());

        // Assets & Data file ??
        // Add to local game ? Or send req to server to create ?
        // Just add to the local game, NOT the DB or the local cache
        TheGame::Instance()->AddGameObject(go);
      }
      else
      {
std::cout << "Failed to create object of type " << m_typename << "\n";
      }
      TheGSEdit::Instance()->OnNewObjectId(ToInt(id));
    }
    else
    {
      OnFailure();
    }
  }

  virtual void OnFailure()
  {
std::cout << "FAILED to get new object ID.\n";
    TheGSEdit::Instance()->OnNewObjectId(-1); // ==> error
  }

private:
  std::string m_typename;
};

GSEdit::GSEdit()
{
  m_mouseDownLeft = false;
  m_mouseDownRight = false;
  m_location = 0; // Set in OnActive 
  m_isPicking = false;
  m_newObjectId = -1;
  m_getDragVec = false;
}

void GSEdit::SetLocs(const Locs& locs)
{
  m_locs = locs;
}

void GSEdit::OnNewObjectId(int id)
{
  m_newObjectId = id;
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
  TheGame::Instance()->UpdateGameObjects();
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
  
  GetVe1SceneGraph()->GetCamera()->Draw();

  if (m_getDragVec)
  {
    m_getDragVec = false;
    // Get up and right vectors 
    Matrix mat;
    mat.ModelView(); // Get Modelview matrix
    m_up = Vec3f(mat[1], mat[5], mat[9]);
    m_right = Vec3f(mat[0], mat[4], mat[8]);
  }

  // Draw axes for selected object
  if (!m_selectedObjects.empty())
  {
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

    PushColour();
    AmjuGL::SetColour(0, 0, 0, 1);

    GameObject* go = *(m_selectedObjects.begin());
    Vec3f p0 = go->GetPos();
    Vec3f p1 = p0 + m_right * 1000.0f;
    AmjuGL::DrawLine(AmjuGL::Vec3(p0.x, p0.y, p0.z), AmjuGL::Vec3(p1.x, p1.y, p1.z));
    p0 = go->GetPos();
    p1 = p0 + m_up * 1000.0f;
    AmjuGL::DrawLine(AmjuGL::Vec3(p0.x, p0.y, p0.z), AmjuGL::Vec3(p1.x, p1.y, p1.z));

    PopColour();
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  }

  if (m_isPicking)
  {
    m_isPicking = false;

    // Check for selected object
std::cout << "PICKING!\n";
    GameObject* obj = PickObject(m_mouseScreen);
    if (obj)
    {
std::cout << " - GOT AN OBJECT! " << obj->GetId() << "\n";

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
    ((GuiText*)GetElementByName(m_gui, "text1"))->SetText(selText);
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

void OnChooseLocOk()
{
  TheGSEdit::Instance()->GoToSelectedLocation();
}

void OnChooseLocCancel()
{
  TheGSEdit::Instance()->ShowLocationList(false);
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

  m_location = GetLocalPlayerLocation();
  std::string loc = "Location: " + ToString(m_location);
  ((GuiText*)m_gui->GetElementByName("location_info"))->SetText(loc);

  // Send request for list of all locations
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_ALL_LOCATIONS);
  TheVe1ReqManager::Instance()->AddReq(new GetLocations(url)); 

  // Hide locations list box until we want to choose a new location

  GuiElement* elem = GetElementByName(m_gui, "location-ok");
  elem->SetCommand(OnChooseLocOk);

  elem = GetElementByName(m_gui, "location-cancel");
  elem->SetCommand(OnChooseLocCancel); 

  ShowLocationList(false);
}

void GSEdit::ShowLocationList(bool b)
{
  GuiElement* locs = GetElementByName(m_gui, "location-list");
  locs->SetVisible(b);
  GuiElement* elem = GetElementByName(m_gui, "location-ok");
  elem->SetVisible(b);
  elem = GetElementByName(m_gui, "location-cancel");
  elem->SetVisible(b);
}

void GSEdit::OnDeactive()
{
  GSBase::OnDeactive();
  m_gui = 0;
  m_menu = 0;

  // TODO Check for outstanding changes, confirm and save to server
}

bool GSEdit::OnCursorEvent(const CursorEvent& ce)
{
  m_mouseScreen = Vec2f(ce.x, ce.y);

  // If dragging, move selected object(s)
  static float oldx = ce.x;
  static float oldy = ce.y;
  float dx = ce.x - oldx;
  float dy = ce.y - oldy;
  oldx = ce.x;
  oldy = ce.y;

  // Move sel objects
  if (m_mouseDownLeft && 
      !m_selectedObjects.empty())
  {
    for (SelObjects::iterator it = m_selectedObjects.begin(); it != m_selectedObjects.end(); ++it)
    {
      // Move this obj
      GameObject* obj = *it;
      Vec3f pos = obj->GetPos();
      pos += m_right * dx * 100.0f;
      pos += m_up * dy * 100.0f;
      obj->SetPos(pos);
    }
  }

  return false;
}

bool GSEdit::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  m_mouseScreen = Vec2f(mbe.x, mbe.y);

  // Left click: check for clicking on an object
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    m_mouseDownLeft = mbe.isDown;
//std::cout << "Left Mouse button event! " << (m_mouseDownLeft ? "DOWN" : "UP") << "\n";
    m_isPicking = mbe.isDown;
    m_getDragVec = true; // TODO if key held down
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

void OnGotoLocation()
{
  TheGSEdit::Instance()->ChooseLocation();
}

void GSEdit::ChooseLocation()
{
  // Show listbox, populate with all locations
  GuiElement* elem = GetElementByName(m_gui, "location-list");
  GuiListBox* lb = dynamic_cast<GuiListBox*>(elem);
  Assert(lb);
  lb->Clear();
  int s = m_locs.size();
  for (int i = 0; i < s; i++)
  {
    const Loc& loc = m_locs[i];
    GuiText* t = new GuiText;
    t->SetText(ToString(loc.m_id) + " \"" + loc.m_name + "\"");
    lb->AddItem(t);
  }
  ShowLocationList(true);
}

void GSEdit::GoToSelectedLocation()
{
  GuiElement* elem = GetElementByName(m_gui, "location-list");
  GuiListBox* lb = dynamic_cast<GuiListBox*>(elem);
  Assert(lb);
  int n = lb->GetNumChildren();
  int id = -1;
  for (int i = 0; i < n; i++)
  {
    GuiElement* child = lb->GetChild(i);
    if (child->IsSelected())
    {
      id = m_locs[i].m_id;
      break;
    }
  }
  if (id != -1)
  {
    SetLocalPlayerLocation(id);
  }
}

class NewObjectCommand : public GuiCommand
{
public:
  NewObjectCommand(const std::string& typeName) : m_typeName(typeName) { }
  virtual bool Do()
  {
std::cout << "Create new object of type " << m_typeName << "\n";

    // Create request to get new ID. Make obj when we get response.
    std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_NEW_OBJECT_ID);
    url += "&type=" + m_typeName;
    TheVe1ReqManager::Instance()->AddReq(new ReqNewObjId(url, m_typeName)); 

    return false;
  }

private:
  std::string m_typeName;
};

class SetPropsCommand : public GuiCommand
{
public:
  SetPropsCommand() {}
  virtual bool Do()
  {
    return false;
  }
};

void GSEdit::CreateContextMenu()
{
  m_menu = new GuiMenu;
  m_menu->SetPos(m_mouseScreen);
  if (m_selectedObjects.empty())
  {
    m_menu->Clear(); 
    m_menu->AddItem(new GuiMenuItem("Go to Location...", OnGotoLocation));
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
std::cout << "Create context menu for one object...\n";
    m_menu->Clear(); 
    m_menu->AddItem(new GuiMenuItem("Set properties...", new SetPropsCommand()));
//    m_menu->AddItem(new GuiMenuItem("Save changes to object", new SaveObjectCommand()));
  }
  else
  {
    // Multi selection
std::cout << "Create context menu for multiple objects...\n";
  }
}

bool GSEdit::OnKeyEvent(const KeyEvent&)
{
  // TODO Set move command etc for objects with keys ??

  return false;
}

}

